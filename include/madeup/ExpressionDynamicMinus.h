#ifndef EXPRESSIONDYNAMICMINUS_H
#define EXPRESSIONDYNAMICMINUS_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDynamicMinus : public Expression {
  public:
    ExpressionDynamicMinus(const std::string &id, td::Co<Expression> right);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    std::string id;
    td::Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
