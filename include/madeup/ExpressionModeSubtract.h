#ifndef EXPRESSIONMODESUBTRACT_H
#define EXPRESSIONMODESUBTRACT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionModeSubtract : public Expression {
  public:
    ExpressionModeSubtract();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
