#ifndef ELEMENTMOVE_H
#define ELEMENTMOVE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMove : public Expression {
  public:
    ExpressionMove();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
