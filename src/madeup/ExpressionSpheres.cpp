#include "madeup/ExpressionSpheres.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionSpheres::ExpressionSpheres() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSpheres::evaluate(Environment &env) const {
  Co<Trimesh> trimesh = env.spheres();
  return Co<Expression>(new ExpressionMesh(trimesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionSpheres::write(ostream &out) const {
  out << "(spheres)";
}

/* ------------------------------------------------------------------------- */

}
