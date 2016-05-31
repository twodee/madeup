#ifndef ELEMENTSCALE_H
#define ELEMENTSCALE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionScale : public Expression {
  public:
    ExpressionScale();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
