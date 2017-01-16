#include "madeup/ExpressionModeAdd.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionModeAdd::ExpressionModeAdd() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionModeAdd::evaluate(Environment &env) const {
  env.add();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionModeAdd::write(ostream &out) const {
  out << "(add)";
}

/* ------------------------------------------------------------------------- */

}
