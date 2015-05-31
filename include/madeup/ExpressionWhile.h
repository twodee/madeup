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

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> value = Co<Expression>(new ExpressionUnit());
      Co<Expression> v = condition->evaluate(env);
      ExpressionBoolean *condition_value = dynamic_cast<ExpressionBoolean *>(v.GetPointer());
      if (!condition_value) {
        throw MessagedException(condition->getSourceLocation().toAnchor() + ": A while loop expects a boolean condition. " + condition->getSource() + " is not boolean.");
      }
      while (condition_value->toBoolean()) {
        value = block->evaluate(env);
        v = condition->evaluate(env);
        condition_value = dynamic_cast<ExpressionBoolean *>(v.GetPointer());
        if (!condition_value) {
          throw MessagedException(condition->getSourceLocation().toAnchor() + ": A while loop expects a boolean condition. " + condition->getSource() + " is not boolean.");
        }
      }

      return value;
    }

    void write(ostream& out) const {
      out << "(while ";
      condition->write(out);
      out << " ";
      block->write(out);
      out << ")";
    }

  private:
    Co<Expression> condition;
    Co<ExpressionBlock> block;
};

/* ------------------------------------------------------------------------- */

}

#endif
