#ifndef ELEMENTSINE_H
#define ELEMENTSINE_H

#include <math.h>

#include "madeup/Expression.h"
#include "madeup/ExpressionDecimal.h"
#include "madeup/ExpressionNumber.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSine : public Expression {
  public:
    ExpressionSine() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> degrees_closure = env["degrees"];
      if (degrees_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function sin expects a value named degrees. No value named degrees is defined.");
      }

      Co<Expression> v = degrees_closure->Evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (number) {
        float value = number->GetDecimal();
        return Co<Expression>(new ExpressionDecimal(sinf(value * td::PI / 180.0f)));
      }

      throw MessagedException(degrees_closure->GetSourceLocation() + ": Function sin expects a number. " + degrees_closure->GetSource() + " is not a number.");
    }

    void Write(ostream& out) const {
      out << "(sin x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
