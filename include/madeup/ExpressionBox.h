#ifndef ELEMENTBOX_H
#define ELEMENTBOX_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBox : public Expression {
  public:
    ExpressionBox();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
