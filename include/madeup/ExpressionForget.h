#ifndef ELEMENTFORGET_H
#define ELEMENTFORGET_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionForget : public Expression {
  public:
    ExpressionForget();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
