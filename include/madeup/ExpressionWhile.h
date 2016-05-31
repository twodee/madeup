#ifndef ELEMENTWHILE_H
#define ELEMENTWHILE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionWhile : public Expression {
  public:
    ExpressionWhile(td::Co<Expression> condition, td::Co<Expression> body);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    td::Co<Expression> condition;
    td::Co<Expression> body;
};

/* ------------------------------------------------------------------------- */

}

#endif
