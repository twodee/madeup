#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionPolarTo.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionUtilities.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPolarTo::ExpressionPolarTo() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPolarTo::evaluate(Environment &env) const {
  float radius = ExpressionUtilities::lookup<ExpressionNumber>("radius", "number", "polarto", env)->toReal();
  float angle = ExpressionUtilities::lookup<ExpressionNumber>("angle", "number", "polarto", env)->toReal();

  float x = radius * cos(angle * td::PI / 180.0f);
  float y = radius * sin(angle * td::PI / 180.0f);
  env.moveTo(x, y, 0.0f);

  return ExpressionUnit::getSingleton();
}

/* ------------------------------------------------------------------------- */

void ExpressionPolarTo::write(ostream &out) const {
  out << "(polarto)";
}

/* ------------------------------------------------------------------------- */

}
