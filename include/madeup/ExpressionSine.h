#ifndef ELEMENTSINE_H
#define ELEMENTSINE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSine : public Expression {
  public:
    ExpressionSine();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
