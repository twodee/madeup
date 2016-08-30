#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDilate.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

#include "twodee/Box.h"
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

  // Assert closed.
  // TODO

  // Assert coplanar.
  int nnodes_open = path.size() - 1;
  bool found_normal = false;
  QVector3<float> polygon_normal;

  for (int i = 0; i < nnodes_open; ++i) {
    int iprev = (i - 1 + nnodes_open) % nnodes_open;
    int inext = (i + 1) % nnodes_open;

    QVector3<float> to_prev = path[iprev].position - path[i].position;
    QVector3<float> to_next = path[inext].position - path[i].position;

    // Either this is a straight angle (to_prev . to_next == 0) or the
    // normal is coincident with what we've seen before.
    if (!to_prev.IsParallel(to_next)) {
      QVector3<float> normal = to_prev.Cross(to_next);
      if (!found_normal) {
        found_normal = true;
        polygon_normal = normal;
      } else if (!polygon_normal.IsParallel(normal)) {
        throw UnlocatedException("I expect the path being dilated to be flat.");
      }
    }
  }

  // Calculate bounding box.
  QVector3<float> min = path[0].position;
  QVector3<float> max = path[0].position;

  for (auto node = path.begin(); node != path.end(); ++node) {
    for (int d = 0; d < 3; ++d) {
      if (node->position[d] < min[d]) {
        min[d] = node->position[d];
      } else if (node->position[d] > max[d]) {
        max[d] = node->position[d];
      }
    }
  }

  // Which dimension has the biggest span? That will be the dimension to
  // minimize.
  int dmax = Box<float, 3>(max, min).GetMaximumDimension();

  // Find which point is minimum.
  int imin = 0;

  for (int i = 1; i < nnodes_open; ++i) {
    if (path[i].position[dmax] <= path[imin].position[dmax]) {
      imin = i;
    }
  }

  // Find vector to node preceding minimum, ignoring the redundant final point.
  int iprev = (imin - 1 + nnodes_open) % nnodes_open;
  QVector3<float> to_prev = path[iprev].position - path[imin].position;

  // Find vector to node succeeding minimum, ignoring the redundant final point.
  int inext = (imin + 1) % nnodes_open;
  QVector3<float> to_next = path[inext].position - path[(inext - 1 + nnodes_open) % nnodes_open].position;

  // Successor may be parallel to predecessor, in which case we're not going to
  // get a good cross product. Advance until we find a non-straight angle.
  // TODO stop loop sometime
  while (to_prev.IsParallel(to_next)) {
    ++inext; 
    to_next = path[inext].position - path[(inext - 1 + nnodes_open) % nnodes_open].position;
  }

  // Find the vector pointing halfway between two vectors. We know it will
  // point to the inside because we picked a minimum.
  QVector3<float> to_halfway = to_prev + to_next;
  to_halfway.Normalize();

  // Flip it to the outside.
  QVector3<float> to_outside = -to_halfway;

  // Find lines parallel to the original path.
  int nlines = path.size() - 1;
  std::vector<Line<float, 3> > lines(nlines);
  std::vector<QVector3<float> > dilated_positions(nnodes_open);
  std::vector<QVector3<float> > aways(nnodes_open);
  std::vector<bool> is_collinear;
  QVector3<float> first_normal;

  for (int li = 0, i = imin; li < nlines; ++li, i = (i + 1) % nnodes_open) {
    int iprev = (i - 1 + nnodes_open) % nnodes_open;
    int inext = (i + 1) % nnodes_open;
    QVector3<float> to_prev = path[iprev].position - path[i].position;
    QVector3<float> to_next = path[inext].position - path[i].position;
    is_collinear.push_back(false);

    while (to_prev.IsParallel(to_next)) {
      is_collinear[i] = true;
      inext = (inext + 1) % nnodes_open;; 
      to_next = path[inext].position - path[(inext - 1 + nnodes_open) % nnodes_open].position;
    }

    QVector3<float> normal = to_prev.Cross(to_next);
    aways[i] = normal.Cross(to_next);

    normal.Normalize();
    aways[i].Normalize();

    if (li == 0) {
      if (to_outside.Dot(aways[i]) < 0.0f) {
        normal = -normal;
      }
      first_normal = normal;
    } else if (normal.Dot(first_normal) < 0) {
      aways[i] = -aways[i];
    }

    if (li > 0 && is_collinear[i]) {
      aways[i] = aways[(i - 1 + nnodes_open) % nnodes_open];
      inext = (i + 1) % nnodes_open;
      to_next = path[inext].position - path[i].position;
    }

    to_next.Normalize();
    dilated_positions[i] = path[i].position + aways[i] * length;

    lines[i] = Line<float, 3>(dilated_positions[i], to_next);
  }

  /* for (int i = 0; i < nlines; ++i) { */
    /* std::cout << "is_collinear[" << i << "]: " << is_collinear[i] << std::endl; */
  /* } */

  std::vector<Node> dilated_path;
  for (int i = 0; i < nlines; ++i) {
    int iprev = (i - 1 + nlines) % nlines;

    // intersect previous line and this one
    Node node = path[i];
    if (is_collinear[i]) {
      node.position = dilated_positions[i]; //lines[i].GetPoint();
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
