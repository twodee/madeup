#ifndef EXPRESSIONMAGNITUDE_H
#define EXPRESSIONMAGNITUDE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMagnitude : public Expression {
  public:
    ExpressionMagnitude();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
