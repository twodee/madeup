#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionExtrude.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionExtrude::ExpressionExtrude() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionExtrude::evaluate(Environment &env) const {
  Co<ExpressionClosure> closure = env["length"];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function extrude expects a value named length. No value named length is defined.");
  }

  Co<Expression> v = closure->evaluate(env);
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!number) {
    throw MessagedException(v->getSourceLocation().toAnchor() + ": Function extrude expects length to be a number, but it is not.");
  }
  float length = number->toReal();

  closure = env["x"];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function extrude expects a value named x. No value named x is defined.");
  }
  v = closure->evaluate(env);
  number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!number) {
    throw MessagedException(v->getSourceLocation().toAnchor() + ": Function extrude expects x to be a number, but it is not.");
  }
  float x = number->toReal();

  closure = env["y"];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function extrude expects a value named y. No value named y is defined.");
  }
  v = closure->evaluate(env);
  number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!number) {
    throw MessagedException(v->getSourceLocation().toAnchor() + ": Function extrude expects y to be a number, but it is not.");
  }
  float y = number->toReal();

  closure = env["z"];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function extrude expects a value named z. No value named z is defined.");
  }
  v = closure->evaluate(env);
  number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!number) {
    throw MessagedException(v->getSourceLocation().toAnchor() + ": Function extrude expects z to be a number, but it is not.");
  }
  float z = number->toReal();

  QVector3<float> axis(x, y, z);
  axis.Normalize();

  Co<Trimesh> trimesh = env.extrude(axis, length);
  return Co<Expression>(new ExpressionMesh(trimesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionExtrude::write(ostream &out) const {
  out << "(extrude)";
}

/* ------------------------------------------------------------------------- */

}
