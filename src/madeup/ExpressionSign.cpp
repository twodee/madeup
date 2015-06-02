#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionSign.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionSign::ExpressionSign() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSign::evaluate(Environment &env) const {
  Co<ExpressionClosure> n_closure = env["n"];
  if (n_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function sign to be given a number n.");
  }

  Co<Expression> n_value = n_closure->evaluate(env);

  ExpressionInteger *n_integer = dynamic_cast<ExpressionInteger *>(n_value.GetPointer());
  if (n_integer) {
    int n = n_integer->toInteger();
    return Co<Expression>(new ExpressionInteger(n > 0 ? 1 : (n < 0 ? -1 : 0)));
  } else {
    ExpressionNumber *n_number = dynamic_cast<ExpressionNumber *>(n_value.GetPointer());
    if (n_number) {
      float n = n_number->toReal();
      return Co<Expression>(new ExpressionInteger(n > 0 ? 1 : (n < 0 ? -1 : 0)));
    } else {
      throw MessagedException(n_closure->getSourceLocation().toAnchor() + ": I expect function sign to be given a number. That thing you gave it wasn't a number.");
    }
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionSign::write(ostream &out) const {
  out << "(sign a b)";
}

/* ------------------------------------------------------------------------- */

}
