#ifndef ELEMENTAXIS_H
#define ELEMENTAXIS_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAxis : public Expression {
  public:
    ExpressionAxis();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
