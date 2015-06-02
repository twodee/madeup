#include "madeup/ExpressionForget.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionForget::ExpressionForget() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionForget::evaluate(Environment &env) const {
  env.forget();
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionForget::write(ostream &out) const {
  out << "(forget)";
}

/* ------------------------------------------------------------------------- */

}
