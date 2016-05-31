#include "madeup/ExpressionPath.h"
#include "madeup/ExpressionNodes.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPath::ExpressionPath() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPath::evaluate(Environment &env) const {
  std::vector<Node> path = env.popPath();
  return Co<Expression>(new ExpressionNodes(path));
}

/* ------------------------------------------------------------------------- */

void ExpressionPath::write(ostream &out) const {
  out << "(PATH)";
}

/* ------------------------------------------------------------------------- */

}
