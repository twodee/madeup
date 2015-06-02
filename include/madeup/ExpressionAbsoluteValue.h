#ifndef EXPRESSIONABSOLUTEVALUE_H
#define EXPRESSIONABSOLUTEVALUE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAbsoluteValue : public Expression {
  public:
    ExpressionAbsoluteValue(Co<Expression> expr);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> expr;
};

/* ------------------------------------------------------------------------- */

}

#endif
