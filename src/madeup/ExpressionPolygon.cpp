#include "madeup/ExpressionPolygon.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPolygon::ExpressionPolygon() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPolygon::evaluate(Environment &env) const {
  env.polygon();
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionPolygon::write(ostream &out) const {
  out << "(polygon)";
}

/* ------------------------------------------------------------------------- */

}