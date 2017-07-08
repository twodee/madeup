#ifndef ELEMENTPOLARTO_H
#define ELEMENTPOLARTO_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPolarTo : public Expression {
  public:
    ExpressionPolarTo();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
