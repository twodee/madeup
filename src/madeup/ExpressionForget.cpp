#include "madeup/ExpressionForget.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionForget::ExpressionForget() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionForget::evaluate(Environment &env) const {
  env.forget();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionForget::write(ostream &out) const {
  out << "(forget)";
}

/* ------------------------------------------------------------------------- */

}
