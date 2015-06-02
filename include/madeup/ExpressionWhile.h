#ifndef ELEMENTWHILE_H
#define ELEMENTWHILE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionWhile : public Expression {
  public:
    ExpressionWhile(Co<Expression> condition, Co<Expression> body);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> condition;
    Co<Expression> body;
};

/* ------------------------------------------------------------------------- */

}

#endif
