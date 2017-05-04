#ifndef EXPRESSIONRETURN_H
#define EXPRESSIONRETURN_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionReturn : public Expression {
  public:
    ExpressionReturn(td::Co<Expression> e);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    td::Co<Expression> e;
};

/* ------------------------------------------------------------------------- */

}

#endif
