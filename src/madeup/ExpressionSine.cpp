#include <math.h>

#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionSine.h"
#include "twodee/Utilities.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionSine::ExpressionSine() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSine::evaluate(Environment &env) const {
  Co<ExpressionClosure> degrees_closure = env["degrees"];
  if (degrees_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function sin expects a value named degrees. No value named degrees is defined.");
  }

  Co<Expression> v = degrees_closure->evaluate(env);

  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (number) {
    float value = number->toReal();
    return Co<Expression>(new ExpressionReal(sinf(value * td::PI / 180.0f)));
  }

  throw MessagedException(degrees_closure->getSourceLocation().toAnchor() + ": Function sin expects a number. " + degrees_closure->getSource() + " is not a number.");
}

/* ------------------------------------------------------------------------- */

void ExpressionSine::write(ostream &out) const {
  out << "(sin x)";
}

/* ------------------------------------------------------------------------- */

}
