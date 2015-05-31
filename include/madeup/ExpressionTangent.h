#ifndef ELEMENTTANGENT_H
#define ELEMENTTANGENT_H

#include <math.h>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionNumber.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionTangent : public Expression {
  public:
    ExpressionTangent() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> degrees_closure = env["degrees"];
      if (degrees_closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function tan expects a value named degrees. No value named degrees is defined.");
      }

      Co<Expression> v = degrees_closure->evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (number) {
        float value = number->toReal();
        return Co<Expression>(new ExpressionReal(tanf(value * td::PI / 180.0f)));
      }

      throw MessagedException(degrees_closure->getSourceLocation().toAnchor() + ": Function tan expects a number. " + degrees_closure->getSource() + " is not a number.");
    }

    void write(ostream& out) const {
      out << "(tan x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
