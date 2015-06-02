#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionEcho.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionEcho::ExpressionEcho() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionEcho::evaluate(Environment &env) const {
  env.echo();
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionEcho::write(ostream &out) const {
  out << "(echo)";
}

/* ------------------------------------------------------------------------- */

}
