#ifndef ELEMENTDOWEL_H
#define ELEMENTDOWEL_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDowel : public Expression {
  public:
    ExpressionDowel();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
