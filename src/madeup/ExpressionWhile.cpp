#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionWhile.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionWhile::ExpressionWhile(Co<Expression> condition, Co<Expression> body) :
  Expression(),
  condition(condition),
  body(body) {
  isEmitter(false);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionWhile::evaluate(Environment &env) const {
  Co<Expression> value = Co<Expression>(ExpressionUnit::getSingleton());
  Co<Expression> v = condition->evaluate(env);
  ExpressionBoolean *condition_value = dynamic_cast<ExpressionBoolean *>(v.GetPointer());
  if (!condition_value) {
    throw MessagedException(condition->getSourceLocation().toAnchor() + ": A while loop expects a boolean condition. " + condition->getSource() + " is not boolean.");
  }

  while (condition_value->toBoolean()) {
    env.checkTimeout(getSourceLocation());
    value = body->evaluate(env);
    v = condition->evaluate(env);
    condition_value = dynamic_cast<ExpressionBoolean *>(v.GetPointer());
    if (!condition_value) {
      throw MessagedException(condition->getSourceLocation().toAnchor() + ": A while loop expects a boolean condition. " + condition->getSource() + " is not boolean.");
    }
  }

  return value;
}

/* ------------------------------------------------------------------------- */

void ExpressionWhile::write(ostream &out) const {
  out << "(while ";
  condition->write(out);
  out << " ";
  body->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
