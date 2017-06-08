#include "madeup/ExpressionPush.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPush::ExpressionPush() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPush::evaluate(Environment &env) const {
  env.getBookmarks().push({env.getTurtle(), env.getTransform()});
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionPush::write(ostream &out) const {
  out << "(push)";
}

/* ------------------------------------------------------------------------- */

}
