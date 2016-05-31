#ifndef EXPRESSIONNOT_H
#define EXPRESSIONNOT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNot : public Expression {
  public:
    ExpressionNot(td::Co<Expression> left);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static td::Co<Expression> evaluate_helper(td::Co<Expression> e,
                                          const std::string &source,
                                          const SourceLocation &location,
                                          Environment &env);

    td::Co<Expression> left;
};

/* ------------------------------------------------------------------------- */

}

#endif
