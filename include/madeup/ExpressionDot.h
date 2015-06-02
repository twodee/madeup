#ifndef ELEMENTDOT_H
#define ELEMENTDOT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDot : public Expression {
  public:
    ExpressionDot();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
