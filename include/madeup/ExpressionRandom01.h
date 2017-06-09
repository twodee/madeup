#ifndef EXPRESSIONRANDOM01_H
#define EXPRESSIONRANDOM01_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRandom01 : public Expression {
  public:
    ExpressionRandom01();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
