#include <math.h>

#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionCosine.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionCosine::ExpressionCosine() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionCosine::evaluate(Environment &env) const {
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

  throw MessagedException(getSourceLocation().toAnchor() + ": Function cos expects a number. " + degrees_closure->getSource() + " is not a number.");
}

/* ------------------------------------------------------------------------- */

void ExpressionCosine::write(ostream &out) const {
  out << "(cos x)";
}

/* ------------------------------------------------------------------------- */

}
