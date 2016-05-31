#ifndef EXPRESSIONINTEGER_H
#define EXPRESSIONINTEGER_H

#include "madeup/ExpressionNumber.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionInteger : public ExpressionNumber {
  public:
    ExpressionInteger(int value);

    td::Co<Expression> evaluate(Environment &env) const;
    int toInteger() const;
    void setInteger(int i);
    float toReal() const;
    void write(ostream &out) const;
    std::string toString() const;

  private:
    int value;
};

/* ------------------------------------------------------------------------- */

}

#endif
