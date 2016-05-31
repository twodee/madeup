#ifndef ELEMENTPUSH_H
#define ELEMENTPUSH_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPush : public Expression {
  public:
    ExpressionPush();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
