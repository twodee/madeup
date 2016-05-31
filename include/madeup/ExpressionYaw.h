#ifndef ELEMENTYAW_H
#define ELEMENTYAW_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionYaw : public Expression {
  public:
    ExpressionYaw();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
