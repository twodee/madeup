#ifndef ELEMENTBOXES_H
#define ELEMENTBOXES_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBoxes : public Expression {
  public:
    ExpressionBoxes();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
