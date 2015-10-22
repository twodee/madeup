#ifndef EXPRESSIONANY_H
#define EXPRESSIONANY_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAny : public Expression {
  public:
    ExpressionAny();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
