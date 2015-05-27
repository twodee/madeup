#ifndef EXPRESSIONDECIMAL_H
#define EXPRESSIONDECIMAL_H

#include "madeup/ExpressionNumber.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDecimal : public ExpressionNumber {
  public:
    ExpressionDecimal(float value) :
      ExpressionNumber(),
      value(value) {
    }

    Co<Expression> Evaluate(Environment& env) {
      return Co<Expression>(new ExpressionDecimal(value));
    }

    float GetDecimal() const {
      return value;
    }

    int GetInteger() const {
      return (int) value;
    }

    void Write(ostream& out) const {
      out << "(DECIMAL " << value << ")";
    }

  private:
    float value;
};

/* ------------------------------------------------------------------------- */

}

#endif
