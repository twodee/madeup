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
    new_node.position = new_node.position;
    mirrored_path.push_back(new_node);
  }
  std::cout << "mirrored_path.size(): " << mirrored_path.size() << std::endl;

  for (vector<Node>::const_reverse_iterator i = path.rbegin(); i != path.rend(); ++i) {
    Node new_node = *i;
    new_node.position = new_node.position.Reflect(axis, pivot);
#if 0
    if (i == path.rbegin()) {
      std::cout << "new_node: " << new_node.position << std::endl;
      if ((i != path.rbegin() && i != --path.rend()) ||
          (i == path.rbegin() && new_node.position.GetDistanceTo(mirrored_path[mirrored_path.size() - 1].position) > 1.0e-3f) ||
          (i == --path.rend() && new_node.position.GetDistanceTo(mirrored_path[0].position) > 1.0e-3f)) {
        std::cout << "keeping" << std::endl;
      } else {
        std::cout << "skipping" << std::endl;
      }
    }
#endif
    if ((i != path.rbegin() && i != --path.rend()) ||
        (i == path.rbegin() && new_node.position.GetDistanceTo(mirrored_path[mirrored_path.size() - 1].position) > 1.0e-3f) ||
        (i == --path.rend() && new_node.position.GetDistanceTo(mirrored_path[0].position) > 1.0e-3f)) {
      mirrored_path.push_back(new_node);
    }
  }
  std::cout << "mirrored_path.size(): " << mirrored_path.size() << std::endl;

  return Co<Expression>(new ExpressionNodes(mirrored_path));
}

/* ------------------------------------------------------------------------- */

void ExpressionMirror::write(ostream &out) const {
  out << "(mirror)";
}

/* ------------------------------------------------------------------------- */

}
