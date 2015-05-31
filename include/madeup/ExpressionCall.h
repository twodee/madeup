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

    void AddParameter(Co<Expression> parameter) {
      parameters.push_back(parameter);
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> closure = env[name];
      if (closure.IsNull()) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": I couldn't find a function or variable named " + name + ".");
      }

      Co<ExpressionDefine> define = closure->GetDefine();
      
      // Make sure there are the correct number of parameters!
      if (parameters.size() != define->GetArity()) {
        std::stringstream ss;
        ss << GetSourceLocation().toAnchor() << ": I expect function " << define->GetName() << " to be given " << define->GetArity() << " parameter";
        if (define->GetArity() != 1) {
          ss << "s";
        }

        if (define->GetArity() > 0) {
          ss << ": " << define->GetFormal(0).GetName();
          for (unsigned int i = 1; i < define->GetArity(); ++i) {
            ss << ", " << define->GetFormal(i).GetName();
          }
        }

        ss << ". But " << parameters.size() << (parameters.size() == 1 ? " was" : " were") << " given.";

        throw MessagedException(ss.str());
      }

      Co<Environment> closure_env(new Environment(*closure->GetEnvironment()));
      for (unsigned int i = 0; i < define->GetArity(); ++i) {
        const FormalParameter &formal = define->GetFormal(i);
        bool is_lazy = formal.GetEvaluationMode() == FormalParameter::LAZY;
        Co<ExpressionDefine> parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.GetName(), is_lazy ? parameters[i] : parameters[i]->Evaluate(env)));

        Co<ExpressionClosure> parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment()));
        parameter_closure->SetSource(parameters[i]->GetSource(), parameters[i]->GetSourceLocation());

        closure_env->Add(define->GetFormal(i).GetName(), parameter_closure);
      }

      try {
        return closure->Evaluate(*closure_env);
      } catch (UnlocatedException e) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": " + e.GetMessage());
      }
    }

    void Write(ostream& out) const {
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
