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

    Co<Expression> evaluate(Environment& env) const {
      return Co<Expression>(new ExpressionInteger(value));
    }

    int toInteger() const {
      return value;
    }

    void setInteger(int i) {
      value = i;
    }

    float toReal() const {
      return value;
    }

    void write(ostream& out) const {
      out << "(INTEGER " << value << ")";
    }

  private:
    int value;
};

/* ------------------------------------------------------------------------- */

}

#endif
