#include "madeup/ExpressionCenter.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionCenter::ExpressionCenter() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionCenter::evaluate(Environment &env) const {
  env.center();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionCenter::write(ostream &out) const {
  out << "(center)";
}

/* ------------------------------------------------------------------------- */

}
