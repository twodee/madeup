#include <sstream>

#include "madeup/ExpressionAbsoluteValue.h"
#include "madeup/ExpressionArray.h"
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
  return evaluate_helper(expr, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionAbsoluteValue::evaluate_helper(Co<Expression> e,
                                                        const std::string &source,
                                                        const SourceLocation &location,
                                                        Environment &env) {
  Co<Expression> value = e->evaluate(env);

  // Integer
  ExpressionInteger *integer = dynamic_cast<ExpressionInteger *>(value.GetPointer());
  if (integer) {
    return Co<Expression>(new ExpressionInteger(abs(integer->toInteger())));
  }

  // Any number
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(value.GetPointer());
  if (number) {
    return Co<Expression>(new ExpressionReal(fabs(number->toReal())));
  }

  // Array
  ExpressionArrayReference *e_array = dynamic_cast<ExpressionArrayReference *>(value.GetPointer());
  if (e_array) {
    int nitems = e_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, evaluate_helper((*e_array->getArray())[i], source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  throw MessagedException(e->getSourceLocation().toAnchor() + ": I don't know how to compute the absolute value of " + e->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionAbsoluteValue::write(ostream &out) const {
  out << "(abs ";
  expr->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
