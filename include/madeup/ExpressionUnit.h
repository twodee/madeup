#ifndef EXPRESSIONUNIT_H
#define EXPRESSIONUNIT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionUnit : public Expression {
  public:
    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

    static td::Co<Expression> getSingleton();

  private:
    ExpressionUnit();

    static td::Co<Expression> singleton;
};

/* ------------------------------------------------------------------------- */

}

#endif
