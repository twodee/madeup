#ifndef ELEMENTCALL_H
#define ELEMENTCALL_H

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"

using std::string;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCall : public Expression {
  public:
    ExpressionCall(const string& name) :
      Expression(),
      name(name),
      parameters() {

    }

    void addParameter(Co<Expression> parameter) {
      parameters.push_back(parameter);
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> closure = env[name];
      if (closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": I couldn't find a function or variable named " + name + ".");
      }

      Co<ExpressionDefine> define = closure->getDefine();
      
      // Make sure there are the correct number of parameters!
      if (parameters.size() != define->getArity()) {
        std::stringstream ss;
        ss << getSourceLocation().toAnchor() << ": I expect function " << define->getName() << " to be given " << define->getArity() << " parameter";
        if (define->getArity() != 1) {
          ss << "s";
        }

        if (define->getArity() > 0) {
          ss << ": " << define->getFormal(0).getName();
          for (unsigned int i = 1; i < define->getArity(); ++i) {
            ss << ", " << define->getFormal(i).getName();
          }
        }

        ss << ". But " << parameters.size() << (parameters.size() == 1 ? " was" : " were") << " given.";

        throw MessagedException(ss.str());
      }

      Co<Environment> closure_env(new Environment(*closure->getEnvironment()));
      /* std::cout << "closure_env: " << *closure_env << std::endl; */
      for (unsigned int i = 0; i < define->getArity(); ++i) {
        const FormalParameter &formal = define->getFormal(i);
        bool is_lazy = formal.getEvaluationMode() == FormalParameter::LAZY;
        Co<ExpressionDefine> parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), is_lazy ? parameters[i] : parameters[i]->evaluate(env)));

        Co<ExpressionClosure> parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment()));
        parameter_closure->setSource(parameters[i]->getSource(), parameters[i]->getSourceLocation());

        closure_env->add(define->getFormal(i).getName(), parameter_closure);
      }

      try {
        return closure->evaluate(*closure_env);
      } catch (UnlocatedException e) {
        throw MessagedException(getSourceLocation().toAnchor() + ": " + e.GetMessage());
      }
    }

    void write(ostream& out) const {
      out << "(call " << name;
      for (vector<Co<Expression> >::const_iterator i = parameters.begin(); i != parameters.end(); ++i) {
        out << " " << *i;
      }
      out << ")";
    }

  private:
    string name;
    vector<Co<Expression> > parameters;
};

/* ------------------------------------------------------------------------- */

}

#endif
