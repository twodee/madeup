#ifndef EXPRESSIONRANDOM_H
#define EXPRESSIONRANDOM_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRandom : public Expression {
  public:
    ExpressionRandom();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
