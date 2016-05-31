#ifndef ELEMENTINVERSESINE_H
#define ELEMENTINVERSESINE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionInverseSine : public Expression {
  public:
    ExpressionInverseSine();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
