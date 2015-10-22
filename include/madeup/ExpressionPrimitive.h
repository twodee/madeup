#ifndef EXPRESSIONPRIMITIVE_H
#define EXPRESSIONPRIMITIVE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPrimitive : public Expression {
  public:
    virtual std::string toString() const = 0;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
