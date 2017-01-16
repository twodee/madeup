#ifndef EXPRESSIONMODEADD_H
#define EXPRESSIONMODEADD_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionModeAdd : public Expression {
  public:
    ExpressionModeAdd();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
