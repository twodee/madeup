#ifndef EXPRESSIONREAL_H
#define EXPRESSIONREAL_H

#include "madeup/ExpressionNumber.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionReal : public ExpressionNumber {
  public:
    ExpressionReal(float value);

    Co<Expression> evaluate(Environment &env) const;
    float toReal() const;
    int toInteger() const;
    void write(ostream &out) const;
    std::string toString() const;

  private:
    float value;
};

/* ------------------------------------------------------------------------- */

}

#endif
