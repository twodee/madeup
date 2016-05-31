#ifndef ELEMENTCOSINE_H
#define ELEMENTCOSINE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCosine : public Expression {
  public:
    ExpressionCosine();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
