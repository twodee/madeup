#ifndef EXPRESSIONDILATE_H
#define EXPRESSIONDILATE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDilate : public Expression {
  public:
    ExpressionDilate();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
