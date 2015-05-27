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

    Co<Expression> Evaluate(Environment& env) {
      env.Push();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(push)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
