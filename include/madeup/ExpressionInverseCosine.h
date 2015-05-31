#ifndef ELEMENTINVERSECOSINE_H
#define ELEMENTINVERSECOSINE_H

#include <math.h>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionNumber.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionInverseCosine : public Expression {
  public:
    ExpressionInverseCosine() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> ratio_closure = env["ratio"];
      if (ratio_closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": I expect function acos to given a value named ratio, but I couldn't find any ratio.");
      }

      Co<Expression> v = ratio_closure->evaluate(env);
      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(ratio_closure->getSourceLocation().toAnchor() + ": I expect the ratio given to acos to be a number. But that thing you gave it is not a number.");
      }
      
      float value = number->toReal();
      return Co<Expression>(new ExpressionReal(acos(value) * 180.0f / td::PI));
    }

    void write(ostream& out) const {
      out << "(acos x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
