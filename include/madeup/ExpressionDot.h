#ifndef ELEMENTDOT_H
#define ELEMENTDOT_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDot : public Expression {
  public:
    ExpressionDot() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      env.Dot();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(dot)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
