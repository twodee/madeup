#ifndef ELEMENTEXTRUDE_H
#define ELEMENTEXTRUDE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionExtrude : public Expression {
  public:
    ExpressionExtrude();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
