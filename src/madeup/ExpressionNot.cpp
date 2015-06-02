#include <sstream>

#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionNot.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionNot::ExpressionNot(Co<Expression> left) :
  Expression(),
  left(left) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNot::evaluate(Environment &env) const {
  Co<Expression> lvalue = left->evaluate(env);

  ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
  
  if (lboolean) {
    return Co<Expression>(new ExpressionBoolean(!lboolean->toBoolean()));
  } else {
    throw MessagedException(left->getSourceLocation().toAnchor() + ": Operator not expects a boolean operand. " + left->getSource() + " is not boolean.");
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionNot::write(ostream &out) const {
  out << "(not ";
  left->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
