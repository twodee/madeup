#ifndef ELEMENTFORGET_H
#define ELEMENTFORGET_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionForget : public Expression {
  public:
    ExpressionForget() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      env.Forget();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(forget)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
