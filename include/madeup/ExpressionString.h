#ifndef EXPRESSIONSTRING_H
#define EXPRESSIONSTRING_H

#include <string>

#include "madeup/Expression.h"

using std::string;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionString : public Expression {
  public:
    ExpressionString(const string& value) :
      Expression(),
      value(value) {
    }

    Co<Expression> evaluate(Environment& env) const {
      return Co<Expression>(new ExpressionString(value));
    }

    string GetString() const {
      return value;
    }

    void write(ostream& out) const {
      out << "(STRING " << value << ")";
    }

  private:
    string value;
};

/* ------------------------------------------------------------------------- */

}

#endif
