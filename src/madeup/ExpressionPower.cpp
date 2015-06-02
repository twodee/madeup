#include <sstream>

#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionPower.h"
#include "madeup/ExpressionReal.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPower::ExpressionPower(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPower::evaluate(Environment &env) const {
  Co<Expression> lvalue = left->evaluate(env);
  Co<Expression> rvalue = right->evaluate(env);

  ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
  ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());

  if (!lnumber) {
    throw MessagedException(left->getSourceLocation().toAnchor() + ": Operator ^ expects a numeric base. " + left->getSource() + " is not a number.");
  }

  if (!rnumber) {
    throw MessagedException(right->getSourceLocation().toAnchor() + ": Operator ^ expects a numeric exponent. " + right->getSource() + " is not a number.");
  }

  ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
  ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
  
  Co<Expression> r;
  if (linteger && rinteger) {
    r = Co<Expression>(new ExpressionInteger((int) pow(linteger->toInteger(), rinteger->toInteger())));
  } else {
    r = Co<Expression>(new ExpressionReal(pow(lnumber->toReal(), rnumber->toReal())));
  }

  return r;
}

/* ------------------------------------------------------------------------- */

void ExpressionPower::write(ostream &out) const {
  out << "(^ ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
