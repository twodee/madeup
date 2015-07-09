#include "madeup/ExpressionDot.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDot::ExpressionDot() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDot::evaluate(Environment &env) const {
  env.dot();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionDot::write(ostream &out) const {
  out << "(dot)";
}

/* ------------------------------------------------------------------------- */

}
