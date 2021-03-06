#include "madeup/ExpressionBlobs.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionBlobs::ExpressionBlobs() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBlobs::evaluate(Environment &env) const {
  Co<ExpressionClosure> closure = env["grain"];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function blobs expects a value named grain. No value named grain is defined.");
  }

  Co<Expression> v = closure->evaluate(env);
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!number) {
    throw MessagedException(v->getSourceLocation().toAnchor() + ": Function blobs expects grain to be a number, but it is not.");
  }
  float grain = number->toReal();

  closure = env["iso"];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function blob expects a value named iso. No value named iso is defined.");
  }
  v = closure->evaluate(env);
  number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!number) {
    throw MessagedException(v->getSourceLocation().toAnchor() + ": Function blobs expects iso to be a number, but it is not.");
  }
  float iso = number->toReal();

  Co<Trimesh> trimesh = env.blobs(grain, iso);
  return Co<Expression>(new ExpressionMesh(trimesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionBlobs::write(ostream &out) const {
  out << "(blobs)";
}

/* ------------------------------------------------------------------------- */

}
