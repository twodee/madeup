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

    Co<Expression> Evaluate(Environment& env) {
      env.Revolve();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(revolve)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
