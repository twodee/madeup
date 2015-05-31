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

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> degrees_closure = env["degrees"];
      if (degrees_closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function cosine expects a value named degrees. No value named degrees is defined.");
      }

      Co<Expression> v = degrees_closure->evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (number) {
        float value = number->toReal();
        return Co<Expression>(new ExpressionReal(cosf(value * td::PI / 180.0f)));
      }

      throw MessagedException(degrees_closure->getSourceLocation().toAnchor() + ": Function cos expects a number. " + degrees_closure->getSource() + " is not a number.");
    }

    void write(ostream& out) const {
      out << "(cos x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
