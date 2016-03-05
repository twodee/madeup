#include "madeup/ExpressionNodes.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionNodes::ExpressionNodes(std::vector<Node> path) :
  Expression(),
  path(path) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNodes::evaluate(Environment &env) const {
  return Co<Expression>(new ExpressionNodes(path));
}

/* ------------------------------------------------------------------------- */

const std::vector<Node> &ExpressionNodes::getPath() const {
  return path;
}

/* ------------------------------------------------------------------------- */

void ExpressionNodes::write(ostream &out) const {
  out << "(TURTLES)";
}

/* ------------------------------------------------------------------------- */

}
