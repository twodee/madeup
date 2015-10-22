#ifndef EXPRESSIONREALDIVIDE_H
#define EXPRESSIONREALDIVIDE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRealDivide : public Expression {
  public:
    ExpressionRealDivide(Co<Expression> left, Co<Expression> right);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static Co<Expression> evaluate_helper(Co<Expression> l,
                                          Co<Expression> r,
                                          const std::string &source,
                                          const SourceLocation &location,
                                          Environment &env);

    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
