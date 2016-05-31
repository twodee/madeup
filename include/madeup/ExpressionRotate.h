#ifndef ELEMENTROTATE_H
#define ELEMENTROTATE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRotate : public Expression {
  public:
    ExpressionRotate();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
