#ifndef ELEMENTIDENTITY_H
#define ELEMENTIDENTITY_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionIdentity : public Expression {
  public:
    ExpressionIdentity();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
