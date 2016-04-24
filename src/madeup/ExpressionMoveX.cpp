#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionMoveX.h"
#include "madeup/ExpressionNumber.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionMoveX::ExpressionMoveX() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionMoveX::evaluate(Environment &env) const {
  Co<ExpressionClosure> distance = env["length"];
  if (distance.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function movex expects a value named length. No value named length is defined.");
  }
  Co<Expression> v = distance->evaluate(env);

  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (number) {
    float value = number->toReal();
    env.movex(value);
    return v;
  } else {
    throw MessagedException(distance->getSourceLocation().toAnchor() + ": Function movex expects a numeric length. " + distance->getSource() + " is not a number.");
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionMoveX::write(ostream &out) const {
  out << "(movex x)";
}

/* ------------------------------------------------------------------------- */

}
