#ifndef ELEMENTECHO_H
#define ELEMENTECHO_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionEcho : public Expression {
  public:
    ExpressionEcho();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
