#include "madeup/ExpressionBoxes.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionBoxes::ExpressionBoxes() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBoxes::evaluate(Environment &env) const {
  Co<Trimesh> trimesh = env.boxes();
  return Co<Expression>(new ExpressionMesh(trimesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionBoxes::write(ostream &out) const {
  out << "(boxes)";
}

/* ------------------------------------------------------------------------- */

}
