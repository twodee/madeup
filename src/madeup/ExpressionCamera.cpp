#include "madeup/ExpressionCamera.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionCamera::ExpressionCamera(const td::Camera &camera) :
  ExpressionPrimitive(),
  camera(camera) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionCamera::evaluate(Environment &env) const {
  return Co<Expression>(new ExpressionCamera(camera));
}

/* ------------------------------------------------------------------------- */

const td::Camera &ExpressionCamera::toCamera() const {
  return camera;
}

/* ------------------------------------------------------------------------- */

void ExpressionCamera::write(ostream &out) const {
  out << "(camera)";
}

/* ------------------------------------------------------------------------- */

std::string ExpressionCamera::toString() const {
  /* return camera */
  return "CAMERA";
}

/* ------------------------------------------------------------------------- */

}
