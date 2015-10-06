#ifndef ELEMENTPRINT_H
#define ELEMENTPRINT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPrint : public Expression {
  public:
    ExpressionPrint();

    static bool print(const Co<Expression>& value);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
