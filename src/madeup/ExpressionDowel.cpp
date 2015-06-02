#include "madeup/ExpressionDowel.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDowel::ExpressionDowel() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDowel::evaluate(Environment &env) const {
  env.dowel();
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionDowel::write(ostream &out) const {
  out << "(dowel)";
}

/* ------------------------------------------------------------------------- */

}
