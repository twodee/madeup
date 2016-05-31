#ifndef EXPRESSIONHOME_H
#define EXPRESSIONHOME_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionHome : public Expression {
  public:
    ExpressionHome();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
