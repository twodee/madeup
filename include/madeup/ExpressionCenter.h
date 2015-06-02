#ifndef EXPRESSIONCENTER_H
#define EXPRESSIONCENTER_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCenter : public Expression {
  public:
    ExpressionCenter();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
