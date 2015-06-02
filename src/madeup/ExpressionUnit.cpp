#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionUnit::ExpressionUnit() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionUnit::evaluate(Environment &env) const {
  return Co<Expression>();
}

/* ------------------------------------------------------------------------- */

void ExpressionUnit::write(ostream &out) const {
  out << "(unit)";
}

/* ------------------------------------------------------------------------- */

}
