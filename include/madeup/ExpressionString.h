#ifndef EXPRESSIONSTRING_H
#define EXPRESSIONSTRING_H

#include <string>

#include "madeup/Expression.h"
#include "madeup/ExpressionPrimitive.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionString : public ExpressionPrimitive {
  public:
    ExpressionString(const std::string &value);

    Co<Expression> evaluate(Environment &env) const;
    std::string getString() const;
    void write(ostream &out) const;
    std::string toString() const;

  private:
    std::string value;
};

/* ------------------------------------------------------------------------- */

}

#endif
