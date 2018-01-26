#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNoise.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionUtilities.h"

#include "twodee/Box.h"
#include "twodee/MessagedException.h"
#include "twodee/Line.h"
#include "twodee/QVector3.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */
 
struct osn_context *ExpressionNoise::noise_context = nullptr;

/* ------------------------------------------------------------------------- */

ExpressionNoise::ExpressionNoise() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNoise::evaluate(Environment &env) const {
  float x = ExpressionUtilities::lookup<ExpressionNumber>("x", "number", "noise", env)->toReal();
  float y = ExpressionUtilities::lookup<ExpressionNumber>("y", "number", "noise", env)->toReal();

  if (noise_context == NULL) {
    open_simplex_noise(37, &noise_context);
  }

  float noise = (float) open_simplex_noise2(noise_context, x, y);

  return Co<Expression>(new ExpressionReal(noise));
}

/* ------------------------------------------------------------------------- */

void ExpressionNoise::write(ostream &out) const {
  out << "(noise path)";
}

/* ------------------------------------------------------------------------- */

}
