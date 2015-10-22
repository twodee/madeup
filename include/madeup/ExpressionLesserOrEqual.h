#ifndef EXPRESSIONLESSEROREQUAL_H
#define EXPRESSIONLESSEROREQUAL_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionLesserOrEqual : public Expression {
  public:
    ExpressionLesserOrEqual(Co<Expression> left, Co<Expression> right);

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
