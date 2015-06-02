#ifndef ELEMENTIF_H
#define ELEMENTIF_H

#include "madeup/Expression.h"
#include "madeup/ExpressionBlock.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionIf : public Expression {
  public:
    ExpressionIf(Co<Expression> condition, Co<ExpressionBlock> then_block, Co<ExpressionBlock> else_block);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> condition;
    Co<ExpressionBlock> then_block;
    Co<ExpressionBlock> else_block;
};

/* ------------------------------------------------------------------------- */

}

#endif
