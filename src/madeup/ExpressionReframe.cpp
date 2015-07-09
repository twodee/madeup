#include <iostream>

#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionReframe.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionReframe::ExpressionReframe() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionReframe::evaluate(Environment &env) const {
  env.reframe();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionReframe::write(ostream &out) const {
  out << "(reframe)";
}

/* ------------------------------------------------------------------------- */

}
