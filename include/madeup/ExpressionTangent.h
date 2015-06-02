#ifndef ELEMENTTANGENT_H
#define ELEMENTTANGENT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionTangent : public Expression {
  public:
    ExpressionTangent();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
