#include "madeup/ExpressionAnd.h"
#include "madeup/ExpressionBoolean.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionAnd::ExpressionAnd(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionAnd::evaluate(Environment &env) const {
  Co<Expression> lvalue = left->evaluate(env);
  Co<Expression> rvalue = right->evaluate(env);

  ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
  ExpressionBoolean *rboolean = dynamic_cast<ExpressionBoolean *>(rvalue.GetPointer());
  
  Co<Expression> value;
  if (lboolean && rboolean) {
    value = Co<Expression>(new ExpressionBoolean(lboolean->toBoolean() && rboolean->toBoolean()));
  } else if (!lboolean) {
    throw MessagedException(left->getSourceLocation().toAnchor() + ": Operator and expects boolean operands. " + left->getSource() + " is not boolean.");
  } else if (!rboolean) {
    throw MessagedException(right->getSourceLocation().toAnchor() + ": Operator or expects boolean operands. " + right->getSource() + " is not boolean.");
  }

  return value;
}

/* ------------------------------------------------------------------------- */

void ExpressionAnd::write(ostream &out) const {
  out << "(and ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
