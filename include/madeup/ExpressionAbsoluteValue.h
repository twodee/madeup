#ifndef EXPRESSIONABSOLUTEVALUE_H
#define EXPRESSIONABSOLUTEVALUE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAbsoluteValue : public Expression {
  public:
    ExpressionAbsoluteValue(td::Co<Expression> expr);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static td::Co<Expression> evaluate_helper(td::Co<Expression> e,
                                          const std::string &source,
                                          const SourceLocation &location,
                                          Environment &env);
    
    td::Co<Expression> expr;
};

/* ------------------------------------------------------------------------- */

}

#endif
