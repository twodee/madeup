#include "madeup/ExpressionDot.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDot::ExpressionDot() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDot::evaluate(Environment &env) const {
  Co<Trimesh> trimesh = env.dot();
  return Co<Expression>(new ExpressionMesh(trimesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionDot::write(ostream &out) const {
  out << "(dot)";
}

/* ------------------------------------------------------------------------- */

}
