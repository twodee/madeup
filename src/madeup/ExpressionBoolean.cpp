#include "madeup/ExpressionBoolean.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionBoolean::ExpressionBoolean(bool value) :
  Expression(),
  value(value) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBoolean::evaluate(Environment &env) const {
  return Co<Expression>(new ExpressionBoolean(value));
}

/* ------------------------------------------------------------------------- */

bool ExpressionBoolean::toBoolean() const {
  return value;
}

/* ------------------------------------------------------------------------- */

void ExpressionBoolean::write(ostream &out) const {
  out << "(BOOLEAN " << (value ? "true" : "false") << ")";
}

/* ------------------------------------------------------------------------- */

}
