#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionRotate.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRotate::ExpressionRotate() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRotate::evaluate(Environment &env) const {
  Co<ExpressionClosure> xyz[3] = {
    env["x"],
    env["y"],
    env["z"],
  };
  float v[3];

  if (xyz[0].IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function rotate expects a value named x. No value named x is defined.");
  }

  if (xyz[1].IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function rotate expects a value named y. No value named y is defined.");
  }

  if (xyz[2].IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function rotate expects a value named z. No value named z is defined.");
  }

  Co<ExpressionClosure> degrees_closure = env["degrees"];
  if (degrees_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function rotate expects a value named degrees. No value named degrees is defined.");
  }

  for (int i = 0; i < 3; ++i) {
    Co<Expression> vv = xyz[i]->evaluate(env);
    ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(vv.GetPointer());
    if (!number) {
      std::stringstream ss;
      ss << xyz[i]->getSourceLocation().toAnchor() << ": Function rotate expects a numeric " << (char) ('x' + i) << " coordinate. " << xyz[i]->getSource() << " is not a number.";
      throw MessagedException(ss.str());
    }
    v[i] = number->toReal();
  }

  Co<Expression> vv = degrees_closure->evaluate(env);
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(vv.GetPointer());
  if (!number) {
    std::stringstream ss;
    ss << degrees_closure->getSourceLocation().toAnchor() << ": Function rotate expects degrees to be a number. " << degrees_closure->getSource() << " is not a number.";
    throw MessagedException(ss.str());
  }
  float degrees = number->toReal();

  env.rotate(v[0], v[1], v[2], degrees);

  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionRotate::write(ostream &out) const {
  out << "(rotate x y z)";
}

/* ------------------------------------------------------------------------- */

}
