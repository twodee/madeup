#ifndef ELEMENTIF_H
#define ELEMENTIF_H

#include "madeup/Expression.h"
#include "madeup/ExpressionBlock.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionIf : public Expression {
  public:
    ExpressionIf(td::Co<Expression> condition, td::Co<ExpressionBlock> then_block, td::Co<ExpressionBlock> else_block);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    td::Co<Expression> condition;
    td::Co<ExpressionBlock> then_block;
    td::Co<ExpressionBlock> else_block;
};

/* ------------------------------------------------------------------------- */

}

#endif
