#ifndef ELEMENTCOSINE_H
#define ELEMENTCOSINE_H

#include <math.h>

#include "madeup/Expression.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCosine : public Expression {
  public:
    ExpressionCosine() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> degrees_closure = env["degrees"];
      if (degrees_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function cosine expects a value named degrees. No value named degrees is defined.");
      }

      Co<Expression> v = degrees_closure->Evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (number) {
        float value = number->GetReal();
        return Co<Expression>(new ExpressionReal(cosf(value * td::PI / 180.0f)));
      }

      throw MessagedException(degrees_closure->GetSourceLocation() + ": Function cos expects a number. " + degrees_closure->GetSource() + " is not a number.");
    }

    void Write(ostream& out) const {
      out << "(cos x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
