#ifndef EXPRESSIONGREATEROREQUAL_H
#define EXPRESSIONGREATEROREQUAL_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionGreaterOrEqual : public Expression {
  public:
    ExpressionGreaterOrEqual(Co<Expression> left, Co<Expression> right);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
