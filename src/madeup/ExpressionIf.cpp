#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionIf.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionIf::ExpressionIf(Co<Expression> condition, Co<ExpressionBlock> then_block, Co<ExpressionBlock> else_block) :
  Expression(),
  condition(condition),
  then_block(then_block),
  else_block(else_block) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionIf::evaluate(Environment &env) const {
  Co<Expression> v = condition->evaluate(env);
  ExpressionBoolean *bexpr = dynamic_cast<ExpressionBoolean *>(v.GetPointer());

  if (!bexpr) {
    throw MessagedException(condition->getSourceLocation().toAnchor() + ": An if statement expects a boolean condition. " + condition->getSource() + " is not boolean.");
  } else {
    if (bexpr->toBoolean()) {
      return then_block->evaluate(env);
    } else if (!else_block.IsNull()) {
      return else_block->evaluate(env);
    } else {
      return Co<Expression>(ExpressionUnit::getSingleton());
    }
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionIf::write(ostream &out) const {
  out << "(if ";
  condition->write(out);
  out << " ";
  then_block->write(out);
  if (!else_block.IsNull()) {
    out << " ";
    else_block->write(out);
  }
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
