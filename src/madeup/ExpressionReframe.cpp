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
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionReframe::write(ostream &out) const {
  out << "(reframe)";
}

/* ------------------------------------------------------------------------- */

}
