#ifndef ELEMENTLOG_H
#define ELEMENTLOG_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionLog : public Expression {
  public:
    ExpressionLog();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static td::Co<Expression> evaluate_helper(td::Co<Expression> base,
                                          td::Co<Expression> x,
                                          const std::string &source,
                                          const SourceLocation &location,
                                          Environment &env);
};

/* ------------------------------------------------------------------------- */

}

#endif
