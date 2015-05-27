#ifndef EXPRESSIONNUMBER_H
#define EXPRESSIONNUMBER_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNumber : public Expression {
  public:
    ExpressionNumber() :
      Expression() {
    }

    virtual Co<Expression> Evaluate(Environment& env) = 0;
    virtual float GetDecimal() const = 0;
    virtual int GetInteger() const = 0;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
