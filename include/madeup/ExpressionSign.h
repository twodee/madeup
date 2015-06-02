#ifndef EXPRESSIONSIGN_H
#define EXPRESSIONSIGN_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSign : public Expression {
  public:
    ExpressionSign();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
