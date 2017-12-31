#ifndef EXPRESSIONTRIMESH_H
#define EXPRESSIONTRIMESH_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionTrimesh : public Expression {
  public:
    ExpressionTrimesh();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
