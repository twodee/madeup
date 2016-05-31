#ifndef ELEMENTBLOBS_H
#define ELEMENTBLOBS_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBlobs : public Expression {
  public:
    ExpressionBlobs();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
