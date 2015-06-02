#ifndef ELEMENTLOG_H
#define ELEMENTLOG_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionLog : public Expression {
  public:
    ExpressionLog();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
