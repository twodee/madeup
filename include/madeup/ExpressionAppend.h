#ifndef EXPRESSIONAPPEND_H
#define EXPRESSIONAPPEND_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAppend : public Expression {
  public:
    ExpressionAppend();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
