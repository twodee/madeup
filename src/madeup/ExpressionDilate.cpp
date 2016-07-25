#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDilate.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"
#include "twodee/Line.h"
#include "twodee/QVector3.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDilate::ExpressionDilate() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDilate::evaluate(Environment &env) const {
  Co<ExpressionClosure> length_closure = env["length"];
  if (length_closure.IsNull()) {
    throw UnlocatedException("I expect function dilate to given a parameter named length. No value named length was given.");
  }

  Co<Expression> length_value = length_closure->evaluate(env);

  ExpressionNumber *length_number = dynamic_cast<ExpressionNumber *>(length_value.GetPointer());
  if (!length_number) {
    throw UnlocatedException("I expect function dilate to be given a number for its length parameter. " + length_closure->getSource() + " is not a number.");
  }
  float length = length_number->toReal();

  Co<ExpressionClosure> path_closure = env["path"];
  if (path_closure.IsNull()) {
    throw UnlocatedException("I expect function dilate to be given a value named path, but no path was given.");
  }

  Co<Expression> value = path_closure->evaluate(env);

  ExpressionNodes *turtles_value = dynamic_cast<ExpressionNodes *>(value.GetPointer());
  if (!turtles_value) {
    throw MessagedException(path_closure->getSourceLocation().toAnchor() + ": I expect function dilate to be given a path, but that's not what you gave it.");
    return ExpressionUnit::getSingleton();
  }

  // TODO: handle open paths
  // assumes closed
 
  const std::vector<Node> &path = turtles_value->getPath();
  int nnodes = path.size() - 1;
  std::vector<Line<float, 3> > lines;
  std::vector<bool> is_collinear;
  QVector3<float> first_normal;

  for (int i = 0; i < nnodes; ++i) {
    /* std::cout << "i: " << i << std::endl; */
    int iprev = (i - 1 + nnodes) % nnodes;
    int inext = (i + 1) % nnodes;
    QVector3<float> to_prev = path[iprev].position - path[i].position;
    QVector3<float> to_next = path[inext].position - path[i].position;
    is_collinear.push_back(false);

    to_prev.Normalize();
    to_next.Normalize();
    
    std::cout << "to_prev.Dot(to_next): " << to_prev.Dot(to_next) << std::endl;
    while (1.0f - fabs(to_prev.Dot(to_next)) < 0.01f) {
      is_collinear[i] = true;
      ++inext; 
      to_next = path[inext].position - path[(inext + nnodes - 1) % nnodes].position;
      to_next.Normalize();
      std::cout << "inext: " << inext << std::endl;
    }

    QVector3<float> normal = to_prev.Cross(to_next);

    if (i == 0) {
      first_normal = normal;
    } else if (normal.Dot(first_normal) < 0) {
      normal = -normal;
    }

    QVector3<float> away = normal.Cross(to_next);
    away.Normalize();

    Node a = path[i];
    a.position += away * length;
    /* std::cout << "a.position: " << a.position << std::endl; */
    /* dilated_path.push_back(a); */

    /* Node b = path[inext]; */
    /* b.position += away * 0.25f; */
    /* std::cout << "b.position: " << b.position << std::endl; */
    /* dilated_path.push_back(b); */

    lines.push_back(Line<float, 3>(a.position, to_next));
  }

  std::vector<Node> dilated_path;
  for (int i = 0; i < lines.size(); ++i) {
    int iprev = (i - 1 + lines.size()) % lines.size();
    /* int inext = (i + 1) % lines.size(); */

    // intersect previous line and this one
    Node node = path[i];
    if (is_collinear[i]) {
      node.position = lines[i].GetPoint();
    } else {
      node.position = lines[i].ClosestPointTo(lines[iprev]);
    }

    // node.position = intersection;
    dilated_path.push_back(node);
  }

  if (dilated_path.size() > 0) {
    dilated_path.push_back(dilated_path[0]);
  }

  return Co<Expression>(new ExpressionNodes(dilated_path));
}

/* ------------------------------------------------------------------------- */

void ExpressionDilate::write(ostream &out) const {
  out << "(dilate path)";
}

/* ------------------------------------------------------------------------- */

}
