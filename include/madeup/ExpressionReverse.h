#ifndef EXPRESSIONREVERSE_H
#define EXPRESSIONREVERSE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionReverse : public Expression {
  public:
    ExpressionReverse();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
