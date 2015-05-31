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

    Co<Expression> evaluate(Environment& env) const {
      return Co<Expression>(new ExpressionReal(value));
    }

    float toReal() const {
      return value;
    }

    int toInteger() const {
      return (int) value;
    }

    void write(ostream& out) const {
      out << "(REAL " << value << ")";
    }

  private:
    float value;
};

/* ------------------------------------------------------------------------- */

}

#endif
