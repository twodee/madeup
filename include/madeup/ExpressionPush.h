#ifndef ELEMENTPUSH_H
#define ELEMENTPUSH_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPush : public Expression {
  public:
    ExpressionPush() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      env.push();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(push)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
