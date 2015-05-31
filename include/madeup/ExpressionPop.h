#ifndef ELEMENTPOP_H
#define ELEMENTPOP_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPop : public Expression {
  public:
    ExpressionPop() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      env.pop();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(pop)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
