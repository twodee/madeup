#ifndef ELEMENTWHERE_H
#define ELEMENTWHERE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionWhere : public Expression {
  public:
    ExpressionWhere();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
