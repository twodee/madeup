#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionMirror.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionUtilities.h"
#include "twodee/MessagedException.h"
#include "twodee/Polyline.h"

using namespace td;
using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionMirror::ExpressionMirror() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionMirror::evaluate(Environment &env) const {
  Co<ExpressionNodes> nodes = ExpressionUtilities::lookup<ExpressionNodes>("path", "path", "mirror", env);
  Co<ExpressionArrayReference> axis_ref = ExpressionUtilities::lookup<ExpressionArrayReference>("axis", "array", "mirror", env);
  Co<ExpressionArrayReference> pivot_ref = ExpressionUtilities::lookup<ExpressionArrayReference>("pivot", "array", "mirror", env);

  if (axis_ref->getArray()->getSize() != 3) {
    std::stringstream ss;
    ss << "I expect function mirror's axis array to have size 3. ";
    ss << "But axis has size " << axis_ref->getArray()->getSize() << ".";
    throw UnlocatedException(ss.str());
  }

  if (pivot_ref->getArray()->getSize() != 3) {
    std::stringstream ss;
    ss << "I expect function mirror's pivot array to have size 3. ";
    ss << "But pivot has size " << pivot_ref->getArray()->getSize() << ".";
    throw UnlocatedException(ss.str());
  }

  QVector3<float> axis;
  QVector3<float> pivot;

  for (int i = 0; i < 3; ++i) {
    Co<Expression> element = axis_ref->getArray()->operator[](i);
    ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(element.GetPointer());
    if (!number) {
      std::stringstream ss;
      ss << "I expect function mirror's axis parameter to be composed of numbers. But axis[" << i << "] is not a number.";
      throw UnlocatedException(ss.str());
    }
    axis[i] = number->toReal();

    element = pivot_ref->getArray()->operator[](i);
    number = dynamic_cast<ExpressionNumber *>(element.GetPointer());
    if (!number) {
      std::stringstream ss;
      ss << "I expect function mirror's pivot parameter to be composed of numbers. But pivot[" << i << "] is not a number.";
      throw UnlocatedException(ss.str());
    }
    pivot[i] = number->toReal();
  }

  const vector<Node> &path = nodes->getPath();
  vector<Node> mirrored_path;
  for (vector<Node>::const_iterator i = path.begin(); i != path.end(); ++i) {
    Node new_node = *i;
    mirrored_path.push_back(new_node);
  }

  for (vector<Node>::const_reverse_iterator i = path.rbegin(); i != path.rend(); ++i) {
    Node new_node = *i;
    new_node.position = new_node.position.Reflect(axis, pivot);
    mirrored_path.push_back(new_node);
  }

  // Eliminate the middle elements if they coalesce.
  vector<Node>::iterator fore = mirrored_path.begin() + (path.size() - 1);
  vector<Node>::iterator aft = fore + 1;
  if (fore->position.DistanceTo(aft->position) < 1.0e-3f) {
    mirrored_path.erase(mirrored_path.erase(fore));
  }
 
  // Eliminate the first and last elements if they coalesce. But we
  // need to insert a dummy copy of the second node to maintain the
  // loop.
  fore = mirrored_path.end() - 1;
  aft = mirrored_path.begin();
  if (fore->position.DistanceTo(aft->position) < 1.0e-3f) {
    mirrored_path.erase(fore);
    mirrored_path.erase(aft);
    mirrored_path.push_back(*mirrored_path.begin());
  }

  return Co<Expression>(new ExpressionNodes(mirrored_path));
}

/* ------------------------------------------------------------------------- */

void ExpressionMirror::write(ostream &out) const {
  out << "(mirror)";
}

/* ------------------------------------------------------------------------- */

}
