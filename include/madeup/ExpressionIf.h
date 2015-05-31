#ifndef ELEMENTIF_H
#define ELEMENTIF_H

#include "madeup/ExpressionBlock.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionBoolean.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionIf : public Expression {
  public:
    ExpressionIf(Co<Expression> condition, Co<ExpressionBlock> then_block, Co<ExpressionBlock> else_block) :
      Expression(),
      condition(condition),
      then_block(then_block),
      else_block(else_block) {
    }

    Co<Expression> evaluate(Environment& env) const {
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
          return Co<Expression>(new ExpressionUnit());
        }
      }
    }

    void write(ostream& out) const {
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

  private:
    Co<Expression> condition;
    Co<ExpressionBlock> then_block;
    Co<ExpressionBlock> else_block;
};

/* ------------------------------------------------------------------------- */

}

#endif
