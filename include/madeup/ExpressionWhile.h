#ifndef ELEMENTWHILE_H
#define ELEMENTWHILE_H

#include "madeup/ExpressionBlock.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionBoolean.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionWhile : public Expression {
  public:
    ExpressionWhile(Co<Expression> condition, Co<ExpressionBlock> block) :
      Expression(),
      condition(condition),
      block(block) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> value = Co<Expression>(new ExpressionUnit());
      Co<Expression> v = condition->Evaluate(env);
      ExpressionBoolean *condition_value = dynamic_cast<ExpressionBoolean *>(v.GetPointer());
      if (!condition_value) {
        throw MessagedException(condition->GetSourceLocation().toAnchor() + ": A while loop expects a boolean condition. " + condition->GetSource() + " is not boolean.");
      }
      while (condition_value->GetBoolean()) {
        value = block->Evaluate(env);
        v = condition->Evaluate(env);
        condition_value = dynamic_cast<ExpressionBoolean *>(v.GetPointer());
        if (!condition_value) {
          throw MessagedException(condition->GetSourceLocation().toAnchor() + ": A while loop expects a boolean condition. " + condition->GetSource() + " is not boolean.");
        }
      }

      return value;
    }

    void Write(ostream& out) const {
      out << "(while ";
      condition->Write(out);
      out << " ";
      block->Write(out);
      out << ")";
    }

  private:
    Co<Expression> condition;
    Co<ExpressionBlock> block;
};

/* ------------------------------------------------------------------------- */

}

#endif
