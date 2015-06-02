#ifndef EXPRESSIONMIN_H
#define EXPRESSIONMIN_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMin : public Expression {
  public:
    ExpressionMin();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
