#include "madeup/ExpressionMesh.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionMesh::ExpressionMesh(Co<Trimesh> mesh) :
  mesh(mesh) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionMesh::evaluate(Environment &env) const {
  return Co<Expression>(new ExpressionMesh(mesh));
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> ExpressionMesh::toMesh() const {
  return mesh;
}

/* ------------------------------------------------------------------------- */

void ExpressionMesh::write(ostream &out) const {
  out << "(MESH)";
}

/* ------------------------------------------------------------------------- */

}
