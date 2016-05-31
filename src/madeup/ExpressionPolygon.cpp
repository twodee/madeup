#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionPolygon.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPolygon::ExpressionPolygon() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPolygon::evaluate(Environment &env) const {
  Co<ExpressionClosure> is_flipped_closure = env["flip"];
  if (is_flipped_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function polygon to be given a boolean parameter named flip, but no flip was given.");
  }

  Co<Expression> is_flipped_value = is_flipped_closure->evaluate(env);
  ExpressionBoolean *is_flipped_boolean = dynamic_cast<ExpressionBoolean *>(is_flipped_value.GetPointer());
  if (!is_flipped_boolean) {
    throw MessagedException(is_flipped_value->getSourceLocation().toAnchor() + ": I expect function polygon to be given a boolean parameter named flip, but what you gave isn't a boolean.");
  }
  bool is_flipped = is_flipped_boolean->toBoolean();

  Co<Trimesh> trimesh = env.polygon(is_flipped);
  return Co<Expression>(new ExpressionMesh(trimesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionPolygon::write(ostream &out) const {
  out << "(polygon)";
}

/* ------------------------------------------------------------------------- */

}
