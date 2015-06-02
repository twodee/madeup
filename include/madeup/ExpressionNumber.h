#ifndef EXPRESSIONNUMBER_H
#define EXPRESSIONNUMBER_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNumber : public Expression {
  public:
    ExpressionNumber();

    virtual float toReal() const = 0;
    virtual int toInteger() const = 0;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
