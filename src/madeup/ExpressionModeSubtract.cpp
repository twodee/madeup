#include "madeup/ExpressionModeSubtract.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionModeSubtract::ExpressionModeSubtract() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionModeSubtract::evaluate(Environment &env) const {
  env.subtract();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionModeSubtract::write(ostream &out) const {
  out << "(subtract)";
}

/* ------------------------------------------------------------------------- */

}
