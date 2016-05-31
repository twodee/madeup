#include <sstream>

#include "madeup/ExpressionReal.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionReal::ExpressionReal(float value) :
  ExpressionNumber(),
  value(value) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionReal::evaluate(Environment &env) const {
  return Co<Expression>(new ExpressionReal(value));
}

/* ------------------------------------------------------------------------- */

float ExpressionReal::toReal() const {
  return value;
}

/* ------------------------------------------------------------------------- */

int ExpressionReal::toInteger() const {
  return (int) value;
}

/* ------------------------------------------------------------------------- */

void ExpressionReal::write(ostream &out) const {
  out << "(REAL " << value << ")";
}

/* ------------------------------------------------------------------------- */

std::string ExpressionReal::toString() const {
  std::stringstream ss;
  ss << value;
  return ss.str(); 
}

/* ------------------------------------------------------------------------- */

}
