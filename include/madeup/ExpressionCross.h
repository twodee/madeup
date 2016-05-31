#ifndef EXPRESSIONCROSS_H
#define EXPRESSIONCROSS_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCross : public Expression {
  public:
    ExpressionCross();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
