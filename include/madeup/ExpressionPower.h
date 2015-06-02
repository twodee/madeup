#ifndef EXPRESSIONPOWER_H
#define EXPRESSIONPOWER_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPower : public Expression {
  public:
    ExpressionPower(Co<Expression> left, Co<Expression> right);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
