#ifndef ELEMENTTUBE_H
#define ELEMENTTUBE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionTube : public Expression {
  public:
    ExpressionTube();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
