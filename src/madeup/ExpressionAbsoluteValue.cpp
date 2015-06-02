#include <sstream>

#include "madeup/ExpressionAbsoluteValue.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionReal.h"
#include "twodee/MessagedException.h"

using namespace td;
using std::ostringstream;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionAbsoluteValue::ExpressionAbsoluteValue(Co<Expression> expr) :
  Expression(),
  expr(expr) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionAbsoluteValue::evaluate(Environment &env) const {
  Co<Expression> value = expr->evaluate(env);

  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(value.GetPointer());

  if (!number) {
    throw MessagedException(expr->getSourceLocation().toAnchor() + ": I can only compute the absolute value of a number. " + expr->getSource() + " is not a number.");
  }

  ExpressionInteger *integer = dynamic_cast<ExpressionInteger *>(value.GetPointer());
  
  Co<Expression> r;
  if (integer) {
    r = Co<Expression>(new ExpressionInteger(abs(integer->toInteger())));
  } else {
    r = Co<Expression>(new ExpressionReal(fabs(number->toReal())));
  }

  return r;
}

/* ------------------------------------------------------------------------- */

void ExpressionAbsoluteValue::write(ostream &out) const {
  out << "(abs ";
  expr->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
