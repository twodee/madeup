#include <math.h>

#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInverseSine.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionInverseSine::ExpressionInverseSine() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionInverseSine::evaluate(Environment &env) const {
  Co<ExpressionClosure> ratio_closure = env["ratio"];
  if (ratio_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function asin to given a value named ratio, but I couldn't find any ratio.");
  }

  Co<Expression> v = ratio_closure->evaluate(env);
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!number) {
    throw MessagedException(ratio_closure->getSourceLocation().toAnchor() + ": I expect the ratio given to asin to be a number. But that thing you gave it is not a number.");
  }
  
  float value = number->toReal();
  return Co<Expression>(new ExpressionReal(asin(value) * 180.0f / td::PI));
}

/* ------------------------------------------------------------------------- */

void ExpressionInverseSine::write(ostream &out) const {
  out << "(asin x)";
}

/* ------------------------------------------------------------------------- */

}
