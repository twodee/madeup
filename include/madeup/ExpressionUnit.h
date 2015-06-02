#ifndef EXPRESSIONUNIT_H
#define EXPRESSIONUNIT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionUnit : public Expression {
  public:
    ExpressionUnit();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
