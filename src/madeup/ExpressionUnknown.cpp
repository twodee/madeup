#include "madeup/ExpressionUnknown.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionUnknown::ExpressionUnknown() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionUnknown::evaluate(Environment &env) const {
  return Co<Expression>(new ExpressionUnknown());
}

/* ------------------------------------------------------------------------- */

void ExpressionUnknown::write(ostream &out) const {
  out << "(unknown)";
}

/* ------------------------------------------------------------------------- */

}
