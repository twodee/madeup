#ifndef EXPRESSIONNOT_H
#define EXPRESSIONNOT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNot : public Expression {
  public:
    ExpressionNot(Co<Expression> left);

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
