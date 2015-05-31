#ifndef ELEMENTREVOLVE_H
#define ELEMENTREVOLVE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRevolve : public Expression {
  public:
    ExpressionRevolve() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      env.revolve();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(revolve)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
