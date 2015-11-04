#ifndef EXPRESSIONNORMALIZE_H
#define EXPRESSIONNORMALIZE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNormalize : public Expression {
  public:
    ExpressionNormalize();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
