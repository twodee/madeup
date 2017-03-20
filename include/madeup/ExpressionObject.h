#ifndef EXPRESSIONOBJECT_H
#define EXPRESSIONOBJECT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionObject : public Expression {
  public:
    virtual td::Co<Expression> center() const = 0;
};

/* ------------------------------------------------------------------------- */

}

#endif
