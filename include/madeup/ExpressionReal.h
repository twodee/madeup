#ifndef EXPRESSIONREAL_H
#define EXPRESSIONREAL_H

#include "madeup/ExpressionNumber.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionReal : public ExpressionNumber {
  public:
    ExpressionReal(float value) :
      ExpressionNumber(),
      value(value) {
    }

    Co<Expression> Evaluate(Environment& env) {
      return Co<Expression>(new ExpressionReal(value));
    }

    float GetReal() const {
      return value;
    }

    int GetInteger() const {
      return (int) value;
    }

    void Write(ostream& out) const {
      out << "(REAL " << value << ")";
    }

  private:
    float value;
};

/* ------------------------------------------------------------------------- */

}

#endif
