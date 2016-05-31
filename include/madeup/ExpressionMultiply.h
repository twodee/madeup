#ifndef EXPRESSIONMULTIPLY_H
#define EXPRESSIONMULTIPLY_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMultiply : public Expression {
  public:
    ExpressionMultiply(td::Co<Expression> left, td::Co<Expression> right);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static td::Co<Expression> evaluate_helper(td::Co<Expression> l,
                                          td::Co<Expression> r,
                                          const std::string &source,
                                          const SourceLocation &location,
                                          Environment &env);

    td::Co<Expression> left;
    td::Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
