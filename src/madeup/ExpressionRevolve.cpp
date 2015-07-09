#include "madeup/ExpressionRevolve.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRevolve::ExpressionRevolve() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRevolve::evaluate(Environment &env) const {
  env.revolve();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionRevolve::write(ostream &out) const {
  out << "(revolve)";
}

/* ------------------------------------------------------------------------- */

}
