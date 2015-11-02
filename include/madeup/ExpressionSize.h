#ifndef EXPRESSIONSIZE_H
#define EXPRESSIONSIZE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSize : public Expression {
  public:
    ExpressionSize();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
