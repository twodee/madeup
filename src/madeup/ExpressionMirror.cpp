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
  Co<ExpressionArrayReference> point_ref = ExpressionUtilities::lookup<ExpressionArrayReference>("point", "array", "mirror", env);

  if (axis_ref->getArray()->getSize() != 3) {
    std::stringstream ss;
    ss << "I expect function mirror's axis array to have size 3. ";
    ss << "But axis has size " << axis_ref->getArray()->getSize() << ".";
    throw UnlocatedException(ss.str());
  }

  if (point_ref->getArray()->getSize() != 3) {
    std::stringstream ss;
    ss << "I expect function mirror's point array to have size 3. ";
    ss << "But point has size " << point_ref->getArray()->getSize() << ".";
    throw UnlocatedException(ss.str());
  }

  QVector3<float> axis;
  QVector3<float> point;

  for (int i = 0; i < 3; ++i) {
    Co<Expression> element = axis_ref->getArray()->operator[](i);
    ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(element.GetPointer());
    if (!number) {
      std::stringstream ss;
      ss << "I expect function mirror's axis parameter to be composed of numbers. But axis[" << i << "] is not a number.";
      throw UnlocatedException(ss.str());
    }
    axis[i] = number->toReal();

    element = point_ref->getArray()->operator[](i);
    number = dynamic_cast<ExpressionNumber *>(element.GetPointer());
    if (!number) {
      std::stringstream ss;
      ss << "I expect function mirror's point parameter to be composed of numbers. But point[" << i << "] is not a number.";
      throw UnlocatedException(ss.str());
    }
    point[i] = number->toReal();
  }

  const vector<Node> &path = nodes->getPath();
  vector<Node> mirrored_path;
  for (vector<Node>::const_reverse_iterator i = path.rbegin(); i != path.rend(); ++i) {
    Node new_node = *i;
    new_node.position = new_node.position.Reflect(axis, point);
    mirrored_path.push_back(new_node);
  }

  return Co<Expression>(new ExpressionNodes(mirrored_path));
}

/* ------------------------------------------------------------------------- */

void ExpressionMirror::write(ostream &out) const {
  out << "(mirror)";
}

/* ------------------------------------------------------------------------- */

}
