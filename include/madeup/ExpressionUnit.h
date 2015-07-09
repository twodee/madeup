#ifndef EXPRESSIONUNIT_H
#define EXPRESSIONUNIT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionUnit : public Expression {
  public:
    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

    static Co<Expression> getSingleton();

  private:
    ExpressionUnit();

    static Co<Expression> singleton;
};

/* ------------------------------------------------------------------------- */

}

#endif
