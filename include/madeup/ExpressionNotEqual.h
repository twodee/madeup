#ifndef EXPRESSIONNOTEQUAL_H
#define EXPRESSIONNOTEQUAL_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNotEqual : public Expression {
  public:
    ExpressionNotEqual(Co<Expression> left, Co<Expression> right);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
