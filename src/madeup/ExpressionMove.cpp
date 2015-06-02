#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionMove.h"
#include "madeup/ExpressionNumber.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionMove::ExpressionMove() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionMove::evaluate(Environment &env) const {
  Co<ExpressionClosure> distance = env["length"];
  if (distance.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function move expects a value named length. No value named length is defined.");
  }
  Co<Expression> v = distance->evaluate(env);

  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (number) {
    float value = number->toReal();
    env.move(value);
    return v;
  } else {
    throw MessagedException(distance->getSourceLocation().toAnchor() + ": Function move expects a numeric length. " + distance->getSource() + " is not a number.");
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionMove::write(ostream &out) const {
  out << "(move x)";
}

/* ------------------------------------------------------------------------- */

}
