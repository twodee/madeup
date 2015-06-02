#include "madeup/ExpressionPop.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPop::ExpressionPop() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPop::evaluate(Environment &env) const {
  env.pop();
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionPop::write(ostream &out) const {
  out << "(pop)";
}

/* ------------------------------------------------------------------------- */

}
