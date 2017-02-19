#ifndef ELEMENTLOFT_H
#define ELEMENTLOFT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionLoft : public Expression {
  public:
    ExpressionLoft();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
