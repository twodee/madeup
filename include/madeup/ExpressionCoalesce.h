#ifndef EXPRESSIONCOALESCE_H
#define EXPRESSIONCOALESCE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCoalesce : public Expression {
  public:
    ExpressionCoalesce();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
