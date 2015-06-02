#ifndef EXPRESSIONSTRING_H
#define EXPRESSIONSTRING_H

#include <string>

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionString : public Expression {
  public:
    ExpressionString(const std::string &value);

    Co<Expression> evaluate(Environment &env) const;
    string getString() const;
    void write(ostream &out) const;

  private:
    std::string value;
};

/* ------------------------------------------------------------------------- */

}

#endif
