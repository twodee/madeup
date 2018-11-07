#ifndef ELEMENTNETWORK_H
#define ELEMENTNETWORK_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNetwork : public Expression {
  public:
    ExpressionNetwork();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
