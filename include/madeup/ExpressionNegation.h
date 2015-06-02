#ifndef EXPRESSIONNEGATION_H
#define EXPRESSIONNEGATION_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNegation : public Expression {
  public:
    ExpressionNegation(Co<Expression> left);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> left;
};

/* ------------------------------------------------------------------------- */

}

#endif
