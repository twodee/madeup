#ifndef ELEMENTSURFACE_H
#define ELEMENTSURFACE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSurface : public Expression {
  public:
    ExpressionSurface();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
