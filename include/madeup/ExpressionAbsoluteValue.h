#ifndef EXPRESSIONABSOLUTEVALUE_H
#define EXPRESSIONABSOLUTEVALUE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAbsoluteValue : public Expression {
  public:
    ExpressionAbsoluteValue(Co<Expression> expr);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static Co<Expression> evaluate_helper(Co<Expression> e,
                                          const std::string &source,
                                          const SourceLocation &location,
                                          Environment &env);
    
    Co<Expression> expr;
};

/* ------------------------------------------------------------------------- */

}

#endif
