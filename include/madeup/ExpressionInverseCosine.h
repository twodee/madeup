#ifndef ELEMENTINVERSECOSINE_H
#define ELEMENTINVERSECOSINE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionInverseCosine : public Expression {
  public:
    ExpressionInverseCosine();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
