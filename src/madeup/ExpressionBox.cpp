#include "madeup/ExpressionBox.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionBox::ExpressionBox() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBox::evaluate(Environment &env) const {
  Co<Trimesh> trimesh = env.box();
  return Co<Expression>(new ExpressionMesh(trimesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionBox::write(ostream &out) const {
  out << "(box)";
}

/* ------------------------------------------------------------------------- */

}
