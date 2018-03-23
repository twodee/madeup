#include "madeup/ExpressionInteger.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionInteger::ExpressionInteger(int value) :
  ExpressionNumber(),
  value(value) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionInteger::evaluate(Environment &env) const {
  return Co<Expression>(new ExpressionInteger(value));
}

/* ------------------------------------------------------------------------- */

int ExpressionInteger::toInteger() const {
  return value;
}

/* ------------------------------------------------------------------------- */

void ExpressionInteger::setInteger(int i) {
  value = i;
}

/* ------------------------------------------------------------------------- */

float ExpressionInteger::toReal() const {
  return value;
}

/* ------------------------------------------------------------------------- */

void ExpressionInteger::write(ostream &out) const {
  out << "(integer " << value << ")";
}

/* ------------------------------------------------------------------------- */

std::string ExpressionInteger::toString() const {
  stringstream ss;
  ss << value;
  return ss.str();
}

/* ------------------------------------------------------------------------- */

}
