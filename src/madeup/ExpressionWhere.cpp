#include <iostream>

#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionWhere.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionWhere::ExpressionWhere() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionWhere::evaluate(Environment &env) const {
  std::cerr << env.getTurtle().position << std::endl;
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionWhere::write(ostream &out) const {
  out << "(where)";
}

/* ------------------------------------------------------------------------- */

}
