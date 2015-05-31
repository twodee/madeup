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

    Co<Expression> evaluate(Environment& env) const {
      env.dot();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(dot)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
