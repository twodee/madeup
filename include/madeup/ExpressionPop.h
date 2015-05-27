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

    Co<Expression> Evaluate(Environment& env) {
      env.Pop();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(pop)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
