#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionDelay.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionUnknown.h"

using std::string;
using std::vector;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDelay::ExpressionDelay() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDelay::evaluate(Environment &env) const {
  env.checkTimeout(getSourceLocation());
}

/* ------------------------------------------------------------------------- */

void ExpressionDelay::write(ostream &out) const {
  out << "(delay)";
}

/* ------------------------------------------------------------------------- */

}
