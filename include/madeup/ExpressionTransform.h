#ifndef EXPRESSIONTRANSFORM_H
#define EXPRESSIONTRANSFORM_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionTransform : public Expression {
  public:
    ExpressionTransform();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
