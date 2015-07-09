#include <iostream>

#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionWhere.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionWhere::ExpressionWhere() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionWhere::evaluate(Environment &env) const {
  std::cerr << env.getTurtle().position << std::endl;
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionWhere::write(ostream &out) const {
  out << "(where)";
}

/* ------------------------------------------------------------------------- */

}
