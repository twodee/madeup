#ifndef ELEMENTREFRAME_H
#define ELEMENTREFRAME_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionReframe : public Expression {
  public:
    ExpressionReframe();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
