#ifndef EXPRESSIONLOOK_H
#define EXPRESSIONLOOK_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionLook : public Expression {
  public:
    ExpressionLook();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
