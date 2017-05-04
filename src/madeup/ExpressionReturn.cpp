#include "madeup/ExpressionReturn.h"
#include "madeup/ExpressionUtilities.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionReturn::ExpressionReturn(Co<Expression> e) :
  Expression(),
  e(e) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionReturn::evaluate(Environment &env) const {
  Co<Expression> return_value = e->evaluate(env);
  throw return_value;
}

/* ------------------------------------------------------------------------- */

void ExpressionReturn::write(ostream &out) const {
  out << "(return e)";
}

/* ------------------------------------------------------------------------- */

}
