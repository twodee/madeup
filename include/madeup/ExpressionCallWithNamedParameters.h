#ifndef EXPRESSIONCALLWITHNAMEDPARAMETERS_H
#define EXPRESSIONCALLWITHNAMEDPARAMETERS_H

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"

using std::string;
using std::pair;

namespace madeup {

typedef map<string, Co<Expression> > bindings_t;
typedef pair<string, Co<Expression> > binding_t;

/* ------------------------------------------------------------------------- */

class ExpressionCallWithNamedParameters : public Expression {
  public:
    ExpressionCallWithNamedParameters(const string& name) :
      Expression(),
      name(name),
      bindings() {
    }

    void addParameter(const std::string& name, Co<Expression> parameter) {
      bindings.insert(binding_t(name, parameter));
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> closure = env[name];
      if (closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": No function named " + name + " is defined.");
      }

      Co<ExpressionDefine> define = closure->getDefine();
      /* Environment sub_env(define->IsDynamicallyScoped() ? env : *closure->getEnvironment()); */

      for (unsigned int i = 0; i < define->getArity(); ++i) {
        // lookup getFormal in bindings list
        // failing, in env
        const FormalParameter& formal = define->getFormal(i);
        bool is_lazy = formal.getEvaluationMode() == FormalParameter::LAZY;

        // Look up parameter name in enumerated bindings first. If we find an explicit parameter with that name, use its value.
        bindings_t::const_iterator match = bindings.find(formal.getName());
        Co<ExpressionDefine> parameter_define;
        Co<ExpressionDefine> parameter_closure;
        if (match != bindings.end()) {
          parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), is_lazy ? match->second : match->second->evaluate(env)));
          parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment()));
          parameter_closure->setSource(match->second->getSource(), match->second->getSourceLocation());
        }

        // If no such parameter was enumerated, checking for a binding with that name in the surrounding environment at the time of the call.
        else if (env.isBound(formal.getName())) {
          Co<Expression> closure = env[formal.getName()];
          parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), is_lazy ? closure : closure->evaluate(env)));
          parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment()));
          parameter_closure->setSource(closure->getSource(), closure->getSourceLocation());
        } else {
          throw MessagedException(getSourceLocation().toAnchor() + ": Function " + define->getName() + " expects a parameter named " + formal.getName() + ". No such parameter was provided and no variable with that name was defined.");
        }

        closure->getEnvironment()->add(define->getFormal(i).getName(), parameter_closure);
      }

      try {
        return closure->evaluate(env);
      } catch (UnlocatedException e) {
        throw MessagedException(getSourceLocation().toAnchor() + ": " + e.GetMessage());
      }
    }

    void write(ostream& out) const {
      out << "(call-with-names " << name;
      for (bindings_t::const_iterator i = bindings.begin(); i != bindings.end(); ++i) {
        out << " " << i->first << " " << *i->second;
      }
      out << ")";
    }

  private:
    string name;
    bindings_t bindings;
};

/* ------------------------------------------------------------------------- */

}

#endif
