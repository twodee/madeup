#ifndef EXPRESSIONREALDIVIDE_H
#define EXPRESSIONREALDIVIDE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRealDivide : public Expression {
  public:
    ExpressionRealDivide(Co<Expression> left, Co<Expression> right);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
