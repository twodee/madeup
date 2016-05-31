#ifndef EXPRESSIONSUBTRACT_H
#define EXPRESSIONSUBTRACT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSubtract : public Expression {
  public:
    ExpressionSubtract(td::Co<Expression> left, td::Co<Expression> right);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;
    static td::Co<Expression> subtract(td::Co<Expression> l,
                                   td::Co<Expression> r,
                                   const std::string &source,
                                   const SourceLocation &location,
                                   Environment &env);

  private:
    td::Co<Expression> left;
    td::Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
