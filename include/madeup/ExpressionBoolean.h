#ifndef EXPRESSIONBOOLEAN_H
#define EXPRESSIONBOOLEAN_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBoolean : public Expression {
  public:
    ExpressionBoolean(bool value);

    Co<Expression> evaluate(Environment &env) const;
    bool toBoolean() const;
    void write(ostream &out) const;

  private:
    bool value;
};

/* ------------------------------------------------------------------------- */

}

#endif
