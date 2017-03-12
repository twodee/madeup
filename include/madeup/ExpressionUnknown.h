#ifndef EXPRESSIONUNKNOWN_H
#define EXPRESSIONUNKNOWN_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionUnknown : public Expression {
  public:
    ExpressionUnknown();
    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
