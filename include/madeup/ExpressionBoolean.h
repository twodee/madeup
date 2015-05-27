#ifndef EXPRESSIONBOOLEAN_H
#define EXPRESSIONBOOLEAN_H

#include "madeup/ExpressionBoolean.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBoolean : public Expression {
  public:
    ExpressionBoolean(bool value) :
      Expression(),
      value(value) {
    }

    Co<Expression> Evaluate(Environment& env) {
      return Co<Expression>(new ExpressionBoolean(value));
    }

    bool GetBoolean() const {
      return value;
    }

    void Write(ostream& out) const {
      out << "(BOOLEAN " << (value ? "true" : "false") << ")";
    }

  private:
    bool value;
};

/* ------------------------------------------------------------------------- */

}

#endif
