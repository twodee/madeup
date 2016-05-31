#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionYaw.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionYaw::ExpressionYaw() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionYaw::evaluate(Environment &env) const {
  Co<ExpressionClosure> degrees_closure = env["degrees"];
  if (degrees_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function yaw expects a value named degrees. No value named degrees is defined.");
  }

  Co<Expression> v = degrees_closure->evaluate(env);
  ExpressionNumber *degrees = dynamic_cast<ExpressionNumber *>(v.GetPointer());

  if (degrees) {
    float value = degrees->toReal();
    env.yaw(value);
    return v;
  } else {
    throw MessagedException(degrees_closure->getSourceLocation().toAnchor() + ": Function yaw expects a number of degrees. " + degrees_closure->getSource() + " is not a number.");
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionYaw::write(ostream &out) const {
  out << "(yaw x)";
}

/* ------------------------------------------------------------------------- */

}
