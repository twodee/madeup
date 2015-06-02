#ifndef ELEMENTDEBUG_H
#define ELEMENTDEBUG_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDebug : public Expression {
  public:
    ExpressionDebug();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
