#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionIdentity.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionIdentity::ExpressionIdentity() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionIdentity::evaluate(Environment &env) const {
  env.identity();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionIdentity::write(ostream &out) const {
  out << "(identity)";
}

/* ------------------------------------------------------------------------- */

}
