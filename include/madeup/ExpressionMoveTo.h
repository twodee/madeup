#ifndef ELEMENTMOVETO_H
#define ELEMENTMOVETO_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMoveTo : public Expression {
  public:
    ExpressionMoveTo();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
