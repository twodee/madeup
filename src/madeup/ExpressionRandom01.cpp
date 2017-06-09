#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionRandom01.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRandom01::ExpressionRandom01() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRandom01::evaluate(Environment &env) const {
  float r = rand() / (float) RAND_MAX;
  return Co<Expression>(new ExpressionReal(r));
}

/* ------------------------------------------------------------------------- */

void ExpressionRandom01::write(ostream &out) const {
  out << "(random min max)";
}

/* ------------------------------------------------------------------------- */

}
