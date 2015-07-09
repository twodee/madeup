#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionReverse.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionReverse::ExpressionReverse() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionReverse::evaluate(Environment &env) const {
  env.reverse();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionReverse::write(ostream &out) const {
  out << "(reverse)";
}

/* ------------------------------------------------------------------------- */

}
