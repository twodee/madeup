#include <sstream>

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
  Co<Expression> lvalue = left->evaluate(env);

  ExpressionInteger *lint = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
  if (lint) {
    return Co<Expression>(new ExpressionInteger(-lint->toInteger()));
  }
  
  ExpressionReal *ldecimal = dynamic_cast<ExpressionReal *>(lvalue.GetPointer());
  if (ldecimal) {
    return Co<Expression>(new ExpressionReal(-ldecimal->toReal()));
  }

  throw MessagedException(left->getSourceLocation().toAnchor() + ": Operator not expects a numeric operand. " + left->getSource() + " is not numeric.");
}

/* ------------------------------------------------------------------------- */

void ExpressionNegation::write(ostream &out) const {
  out << "(negate ";
  left->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
