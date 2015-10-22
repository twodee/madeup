#include "madeup/ExpressionHome.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionHome::ExpressionHome() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionHome::evaluate(Environment &env) const {
  env.home();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionHome::write(ostream &out) const {
  out << "(home)";
}

/* ------------------------------------------------------------------------- */

}
