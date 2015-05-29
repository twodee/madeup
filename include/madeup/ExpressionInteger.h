#ifndef EXPRESSIONINTEGER_H
#define EXPRESSIONINTEGER_H

#include "madeup/ExpressionNumber.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionInteger : public ExpressionNumber {
  public:
    ExpressionInteger(int value) :
      ExpressionNumber(),
      value(value) {
    }

    Co<Expression> Evaluate(Environment& env) {
      return Co<Expression>(new ExpressionInteger(value));
    }

    int GetInteger() const {
      return value;
    }

    void SetInteger(int i) {
      value = i;
    }

    float GetReal() const {
      return value;
    }

    void Write(ostream& out) const {
      out << "(INTEGER " << value << ")";
    }

  private:
    int value;
};

/* ------------------------------------------------------------------------- */

}

#endif
