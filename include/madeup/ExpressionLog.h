#ifndef ELEMENTLOG_H
#define ELEMENTLOG_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionLog : public Expression {
  public:
    ExpressionLog();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static Co<Expression> evaluate_helper(Co<Expression> base,
                                          Co<Expression> x,
                                          const std::string &source,
                                          const SourceLocation &location,
                                          Environment &env);
};

/* ------------------------------------------------------------------------- */

}

#endif
