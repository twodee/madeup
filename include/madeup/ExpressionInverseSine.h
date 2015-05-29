#ifndef ELEMENTINVERSESINE_H
#define ELEMENTINVERSESINE_H

#include <math.h>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionNumber.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionInverseSine : public Expression {
  public:
    ExpressionInverseSine() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> ratio_closure = env["ratio"];
      if (ratio_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": I expect function asin to given a value named ratio, but I couldn't find any ratio.");
      }

      Co<Expression> v = ratio_closure->Evaluate(env);
      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(ratio_closure->GetSourceLocation() + ": I expect the ratio given to asin to be a number. But that thing you gave it is not a number.");
      }
      
      float value = number->GetReal();
      return Co<Expression>(new ExpressionReal(asin(value) * 180.0f / td::PI));
    }

    void Write(ostream& out) const {
      out << "(asin x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
