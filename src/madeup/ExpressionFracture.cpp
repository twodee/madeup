#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionFracture.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionUtilities.h"
#include "twodee/MessagedException.h"
#include "twodee/Polyline.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionFracture::ExpressionFracture() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionFracture::evaluate(Environment &env) const {
  Co<ExpressionNumber> length = ExpressionUtilities::lookup<ExpressionNumber>("length", "number", "fracture", env);
  Co<ExpressionNodes> nodes = ExpressionUtilities::lookup<ExpressionNodes>("path", "path", "fracture", env);

  const std::vector<Node> &path = nodes->getPath();

  if (length->toReal()) {
    Polyline<float> polyline(path.size(), 3 + 3 + 4, true); // TODO closed?
    for (unsigned int i = 0; i < path.size(); ++i) {
      memcpy(&polyline(i)[0], &path[i].position[0], sizeof(float) * 3);
      memcpy(&polyline(i)[3], &path[i].rgb[0], sizeof(float) * 3);
      polyline(i)[6] = path[i].outer_radius;
      polyline(i)[7] = path[i].inner_radius;
      polyline(i)[8] = path[i].energy;
      polyline(i)[9] = path[i].halflife;
    }

    polyline.Fracture(length->toReal(), 3);
    
    std::vector<Node> fractured_path;
    for (int i = 0; i < polyline.GetElementCount(); ++i) {
      Node node;
      node.position = td::QVector3<float>(&polyline(i)[0]);
      node.rgb = td::QVector3<float>(&polyline(i)[3]);
      node.outer_radius = polyline(i)[6];
      node.inner_radius = polyline(i)[7];
      node.energy = polyline(i)[8];
      node.halflife = polyline(i)[9];
      fractured_path.push_back(node);
    }

    return Co<Expression>(new ExpressionNodes(fractured_path));
  } else {
    return Co<Expression>(new ExpressionNodes(path));
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionFracture::write(ostream &out) const {
  out << "(fracture)";
}

/* ------------------------------------------------------------------------- */

}
