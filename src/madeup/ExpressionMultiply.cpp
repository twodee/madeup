#include <sstream>

#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionMultiply.h"
#include "madeup/ExpressionReal.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionMultiply::ExpressionMultiply(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionMultiply::evaluate(Environment &env) const {
  Co<Expression> lvalue = left->evaluate(env);
  Co<Expression> rvalue = right->evaluate(env);

  ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
  ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());
  ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
  ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
  
  // Both are decimals.
  Co<Expression> r;
  if (linteger && rinteger) {
    r = Co<Expression>(new ExpressionInteger(linteger->toInteger() * rinteger->toInteger()));
  } else if (lnumber && rnumber) {
    r = Co<Expression>(new ExpressionReal(lnumber->toReal() * rnumber->toReal()));
  } else {
    throw MessagedException(getSourceLocation().toAnchor() + ": Operator * doesn't know how to multiply " + left->getSource() + " and " + right->getSource() + ".");
  }

  return r;
}

/* ------------------------------------------------------------------------- */

void ExpressionMultiply::write(ostream &out) const {
  out << "(* ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
