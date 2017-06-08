#include "madeup/ExpressionPop.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPop::ExpressionPop() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPop::evaluate(Environment &env) const {
  if (env.getBookmarks().size() == 0) {
    throw UnlocatedException("I tried popping, but I had no corresponding push to which to revert.");
  }

  env.setTurtle(env.getBookmarks().top().turtle);
  env.getBookmarks().pop();
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionPop::write(ostream &out) const {
  out << "(pop)";
}

/* ------------------------------------------------------------------------- */

}
