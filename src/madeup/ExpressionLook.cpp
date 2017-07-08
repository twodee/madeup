#include "madeup/ExpressionCamera.h"
#include "madeup/ExpressionLook.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionUtilities.h"
#include "twodee/MessagedException.h"
#include "twodee/Polyline.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionLook::ExpressionLook() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionLook::evaluate(Environment &env) const {
  Turtle turtle = env.getTurtle();
  turtle.camera = ExpressionUtilities::lookup<ExpressionCamera>("camera", "looking direction", "look", env)->toCamera();
  env.setTurtle(turtle, true);
  return ExpressionUnit::getSingleton();
}

/* ------------------------------------------------------------------------- */

void ExpressionLook::write(ostream &out) const {
  out << "(fracture)";
}

/* ------------------------------------------------------------------------- */

}
