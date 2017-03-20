#include "madeup/ExpressionNodes.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionNodes::ExpressionNodes(std::vector<Node> path) :
  ExpressionObject(),
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

td::Co<Expression> ExpressionNodes::center() const {
  QVector3<float> centroid(0.0f);

  for (auto node : path) {
    centroid += node.position;
  }

  centroid /= path.size();

  Co<ExpressionNodes> centered_nodes(new ExpressionNodes(path));
  for (auto &node : centered_nodes->path) {
    node.position -= centroid;
  }

  return centered_nodes;
}

/* ------------------------------------------------------------------------- */

void ExpressionNodes::write(ostream &out) const {
  out << "(TURTLES)";
}

/* ------------------------------------------------------------------------- */

}
