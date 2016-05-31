#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionRevolve.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRevolve::ExpressionRevolve() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRevolve::evaluate(Environment &env) const {
  Co<Trimesh> trimesh = env.revolve();
  return Co<Expression>(new ExpressionMesh(trimesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionRevolve::write(ostream &out) const {
  out << "(revolve)";
}

/* ------------------------------------------------------------------------- */

}
