#ifndef EXPRESSIONMOVEX_H
#define EXPRESSIONMOVEX_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMoveX : public Expression {
  public:
    ExpressionMoveX();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
