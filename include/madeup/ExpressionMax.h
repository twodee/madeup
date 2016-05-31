#ifndef EXPRESSIONMAX_H
#define EXPRESSIONMAX_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMax : public Expression {
  public:
    ExpressionMax();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
