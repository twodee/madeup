#ifndef EXPRESSIONBOOLEAN_H
#define EXPRESSIONBOOLEAN_H

#include "madeup/Expression.h"
#include "madeup/ExpressionPrimitive.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBoolean : public ExpressionPrimitive {
  public:
    ExpressionBoolean(bool value);

    td::Co<Expression> evaluate(Environment &env) const;
    bool toBoolean() const;
    void write(ostream &out) const;
    std::string toString() const;

  private:
    bool value;
};

/* ------------------------------------------------------------------------- */

}

#endif
