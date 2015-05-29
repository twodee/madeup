#ifndef EXPRESSIONROLL_H
#define EXPRESSIONROLL_H

#include "madeup/Expression.h"
#include "madeup/ExpressionNumber.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRoll : public Expression {
  public:
    ExpressionRoll() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> degrees_closure = env["degrees"];
      if (degrees_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function roll expects a value named degrees. No value named degrees is defined.");
      }

      Co<Expression> v = degrees_closure->Evaluate(env);

      ExpressionNumber *degrees = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (degrees) {
        float value = degrees->GetReal();
        env.Roll(value);
        return v;
      } else {
        throw MessagedException(degrees_closure->GetSourceLocation() + ": Function roll expects a number of degrees. " + degrees_closure->GetSource() + " is not a number.");
      }
    }

    void Write(ostream& out) const {
      out << "(roll x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
