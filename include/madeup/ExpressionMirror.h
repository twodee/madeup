#ifndef EXPRESSIONMIRROR_H
#define EXPRESSIONMIRROR_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMirror : public Expression {
  public:
    ExpressionMirror();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
