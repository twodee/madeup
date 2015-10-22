#include <sstream>

#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionNegation.h"
#include "madeup/ExpressionReal.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionNegation::ExpressionNegation(Co<Expression> left) :
  Expression(),
  left(left) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNegation::evaluate(Environment &env) const {
  return evaluate_helper(left, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNegation::evaluate_helper(Co<Expression> e,
                                                   const std::string &source,
                                                   const SourceLocation &location,
                                                   Environment &env) {
  Co<Expression> value = e->evaluate(env);

  // Integer
  ExpressionInteger *integer = dynamic_cast<ExpressionInteger *>(value.GetPointer());
  if (integer) {
    return Co<Expression>(new ExpressionInteger(-integer->toInteger()));
  }

  // Any number
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(value.GetPointer());
  if (number) {
    return Co<Expression>(new ExpressionReal(-number->toReal()));
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

  throw MessagedException(e->getSourceLocation().toAnchor() + ": I don't know how to negate " + e->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionNegation::write(ostream &out) const {
  out << "(negate ";
  left->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
