#include "madeup/ExpressionPush.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPush::ExpressionPush() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPush::evaluate(Environment &env) const {
  env.push();
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionPush::write(ostream &out) const {
  out << "(push)";
}

/* ------------------------------------------------------------------------- */

}
