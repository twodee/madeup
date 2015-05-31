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

    Co<Expression> evaluate(Environment& env) const {
      env.forget();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(forget)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
