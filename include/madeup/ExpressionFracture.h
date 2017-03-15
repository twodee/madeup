#ifndef EXPRESSIONFRACTURE_H
#define EXPRESSIONFRACTURE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionFracture : public Expression {
  public:
    ExpressionFracture();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
