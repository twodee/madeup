#ifndef EXPRESSIONCOMMENT_H
#define EXPRESSIONCOMMENT_H

#include <string>

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionComment : public Expression {
  public:
    ExpressionComment(const std::string &text);

    Co<Expression> evaluate(Environment &env) const;
    std::string toString() const;

  private:
    std::string text;
};

/* ------------------------------------------------------------------------- */

}

#endif
