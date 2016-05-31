#ifndef ELEMENTPITCH_H
#define ELEMENTPITCH_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPitch : public Expression {
  public:
    ExpressionPitch();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
