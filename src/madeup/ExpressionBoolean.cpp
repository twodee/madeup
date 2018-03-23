#include "madeup/ExpressionBoolean.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionBoolean::ExpressionBoolean(bool value) :
  ExpressionPrimitive(),
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
  out << "(boolean " << (value ? "true" : "false") << ")";
}

/* ------------------------------------------------------------------------- */

std::string ExpressionBoolean::toString() const {
  return value ? "true" : "false"; 
}

/* ------------------------------------------------------------------------- */

}
