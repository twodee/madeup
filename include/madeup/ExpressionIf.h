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

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> v = condition->Evaluate(env);
      ExpressionBoolean *bexpr = dynamic_cast<ExpressionBoolean *>(v.GetPointer());

      if (!bexpr) {
        throw MessagedException(condition->GetSourceLocation() + ": An if statement expects a boolean condition. " + condition->GetSource() + " is not boolean.");
      } else {
        if (bexpr->GetBoolean()) {
          return then_block->Evaluate(env);
        } else if (!else_block.IsNull()) {
          return else_block->Evaluate(env);
        } else {
          return Co<Expression>(new ExpressionUnit());
        }
      }
    }

    void Write(ostream& out) const {
      out << "(if ";
      condition->Write(out);
      out << " ";
      then_block->Write(out);
      if (!else_block.IsNull()) {
        out << " ";
        else_block->Write(out);
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
