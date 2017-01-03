#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionEcho.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionEcho::ExpressionEcho() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionEcho::evaluate(Environment &env) const {
  Co<ExpressionClosure> mesh_closure = env["mesh"];
  if (mesh_closure.IsNull()) {
    // TODO: make sure all functions use Unlocated for non-parameter-specific errors.
    throw UnlocatedException("I expect function echo to be given a value named mesh, but no mesh was given.");
  }

  Co<Expression> value = mesh_closure->evaluate(env);

  ExpressionMesh *mesh_value = dynamic_cast<ExpressionMesh *>(value.GetPointer());
  if (mesh_value) {
    return Co<Expression>(new ExpressionMesh(env.echo(mesh_value->toMesh())));
    /* return ExpressionUnit::getSingleton(); */
  }

  ExpressionNodes *turtles_value = dynamic_cast<ExpressionNodes *>(value.GetPointer());
  if (turtles_value) {
    env.echo(turtles_value->getPath());
    return ExpressionUnit::getSingleton();
  }

  throw MessagedException(mesh_closure->getSourceLocation().toAnchor() + ": I expect function echo to be given a mesh, but that's not what you gave it.");
}

/* ------------------------------------------------------------------------- */

void ExpressionEcho::write(ostream &out) const {
  out << "(echo mesh)";
}

/* ------------------------------------------------------------------------- */

}
