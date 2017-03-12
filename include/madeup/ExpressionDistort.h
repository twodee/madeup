#ifndef ELEMENTDISTORT_H
#define ELEMENTDISTORT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDistort : public Expression {
  public:
    ExpressionDistort();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
