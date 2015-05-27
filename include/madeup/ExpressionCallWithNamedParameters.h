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

    void AddParameter(const std::string& name, Co<Expression> parameter) {
      bindings.insert(binding_t(name, parameter));
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> closure = env[name];
      if (closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": No function named " + name + " is defined.");
      }

      Co<ExpressionDefine> define = closure->GetDefine();
      /* Environment sub_env(define->IsDynamicallyScoped() ? env : *closure->GetEnvironment()); */

      for (unsigned int i = 0; i < define->GetArity(); ++i) {
        // lookup GetFormal in bindings list
        // failing, in env
        const FormalParameter& formal = define->GetFormal(i);
        bool is_lazy = formal.GetEvaluationMode() == FormalParameter::LAZY;

        // Look up parameter name in enumerated bindings first. If we find an explicit parameter with that name, use its value.
        bindings_t::iterator match = bindings.find(formal.GetName());
        Co<ExpressionDefine> parameter_define;
        Co<ExpressionDefine> parameter_closure;
        if (match != bindings.end()) {
          parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.GetName(), is_lazy ? match->second : match->second->Evaluate(env)));
          parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment()));
          parameter_closure->SetSource(match->second->GetSource(),
                                       match->second->GetStartLine(),
                                       match->second->GetStartIndex(),
                                       match->second->GetEndLine(),
                                       match->second->GetEndIndex());
        }

        // If no such parameter was enumerated, checking for a binding with that name in the surrounding environment at the time of the call.
        else if (env.IsBound(formal.GetName())) {
          Co<Expression> closure = env[formal.GetName()];
          parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.GetName(), is_lazy ? closure : closure->Evaluate(env)));
          parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment()));
          parameter_closure->SetSource(closure->GetSource(),
                                       GetStartLine(),
                                       GetStartIndex(),
                                       GetEndLine(),
                                       GetEndIndex());
        } else {
          throw MessagedException(GetSourceLocation() + ": Function " + define->GetName() + " expects a parameter named " + formal.GetName() + ". No such parameter was provided and no variable with that name was defined.");
        }

        closure->GetEnvironment()->Add(define->GetFormal(i).GetName(), parameter_closure);
      }

      try {
        return closure->Evaluate(env);
      } catch (UnlocatedException e) {
        throw MessagedException(GetSourceLocation() + ": " + e.GetMessage());
      }
    }

    void Write(ostream& out) const {
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
