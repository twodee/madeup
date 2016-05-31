#include "madeup/ExpressionPop.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPop::ExpressionPop() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPop::evaluate(Environment &env) const {
  env.pop();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionPop::write(ostream &out) const {
  out << "(pop)";
}

/* ------------------------------------------------------------------------- */

}
