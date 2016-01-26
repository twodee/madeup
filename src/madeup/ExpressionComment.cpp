#include "madeup/ExpressionComment.h"
#include "madeup/ExpressionUnit.h"

using std::string;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionComment::ExpressionComment(const std::string &text) :
  text(text) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionComment::evaluate(Environment &env) const {
  return ExpressionUnit::getSingleton();
}

/* ------------------------------------------------------------------------- */

std::string ExpressionComment::toString() const {
  return text;
}

/* ------------------------------------------------------------------------- */

}
