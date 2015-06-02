#include "madeup/ExpressionRemainder.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRemainder::ExpressionRemainder(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRemainder::evaluate(Environment &env) const {
  Co<Expression> lvalue = left->evaluate(env);
  Co<Expression> rvalue = right->evaluate(env);

  ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
  ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
  
  if (linteger && rinteger) {
    if (rinteger->toInteger() != 0) {
      return Co<Expression>(new ExpressionInteger(linteger->toInteger() % rinteger->toInteger()));
    } else {
      throw MessagedException(right->getSourceLocation().toAnchor() + ": I don't know how to get the remainder of something divided by 0.");
    }
  } else {
    throw MessagedException(getSourceLocation().toAnchor() + ": Operator % doesn't know how to get the remainder of " + left->getSource() + " divided by " + right->getSource() + ".");
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionRemainder::write(ostream &out) const {
  out << "(% ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
