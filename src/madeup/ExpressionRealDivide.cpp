#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionRealDivide.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRealDivide::ExpressionRealDivide(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRealDivide::evaluate(Environment &env) const {
  Co<Expression> lvalue = left->evaluate(env);
  Co<Expression> rvalue = right->evaluate(env);

  ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
  ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());

  Co<Expression> value;
  
  // Make sure both are numbers.
  if (lnumber && rnumber) {
    if (rnumber->toReal() != 0.0) {
      value = Co<Expression>(new ExpressionReal(lnumber->toReal() / rnumber->toReal()));
    } else {
      throw MessagedException(right->getSourceLocation().toAnchor() + ": I don't know how to divide by 0.");
    }
  } else {
    throw MessagedException(getSourceLocation().toAnchor() + ": Operator // doesn't know how to divide " + left->getSource() + " by " + right->getSource() + ".");
  }

  return value;
}

/* ------------------------------------------------------------------------- */

void ExpressionRealDivide::write(ostream &out) const {
  out << "(// ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
