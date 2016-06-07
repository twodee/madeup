#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionTransform.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionTransform::ExpressionTransform() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionTransform::evaluate(Environment &env) const {
  Co<ExpressionClosure> mesh_closure = env["mesh"];
  if (mesh_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function transform to be given a value named mesh, but no mesh was given.");
  }

  Co<Expression> value = mesh_closure->evaluate(env);

  ExpressionMesh *mesh_value = dynamic_cast<ExpressionMesh *>(value.GetPointer());
  if (mesh_value) {
    Trimesh *trimesh(new Trimesh(*mesh_value->toMesh()));
    *trimesh *= env.getTransform();
    return Co<ExpressionMesh>(new ExpressionMesh(Co<Trimesh>(trimesh)));
  }

  /* ExpressionNodes *turtles_value = dynamic_cast<ExpressionNodes *>(value.GetPointer()); */
  /* if (turtles_value) { */
    /* env.transform(turtles_value->getPath()); */
    /* return ExpressionUnit::getSingleton(); */
  /* } */

  throw MessagedException(getSourceLocation().toAnchor() + ": I expect function transform to be given a mesh, but that's not what you gave it.");
}

/* ------------------------------------------------------------------------- */

void ExpressionTransform::write(ostream &out) const {
  out << "(transform mesh)";
}

/* ------------------------------------------------------------------------- */

}
