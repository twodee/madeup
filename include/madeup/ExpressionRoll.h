#ifndef EXPRESSIONROLL_H
#define EXPRESSIONROLL_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRoll : public Expression {
  public:
    ExpressionRoll();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
