#ifndef EXPRESSIONNEGATION_H
#define EXPRESSIONNEGATION_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNegation : public Expression {
  public:
    ExpressionNegation(Co<Expression> left);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static Co<Expression> evaluate_helper(Co<Expression> e,
                                          const std::string &source,
                                          const SourceLocation &location,
                                          Environment &env);

    Co<Expression> left;
};

/* ------------------------------------------------------------------------- */

}

#endif
