#ifndef ELEMENTREVOLVE_H
#define ELEMENTREVOLVE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRevolve : public Expression {
  public:
    ExpressionRevolve();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
