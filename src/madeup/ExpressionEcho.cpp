#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionEcho.h"
#include "madeup/ExpressionMesh.h"
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
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function echo to be given a value named mesh, but no mesh was given.");
  }

  Co<Expression> value = mesh_closure->evaluate(env);

  ExpressionMesh *mesh_value = dynamic_cast<ExpressionMesh *>(value.GetPointer());
  if (mesh_value) {
    env.echo(mesh_value->toMesh());
    return ExpressionUnit::getSingleton();
  } else {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function echo to be given a mesh, but that's not what you gave it.");
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionEcho::write(ostream &out) const {
  out << "(echo mesh)";
}

/* ------------------------------------------------------------------------- */

}
