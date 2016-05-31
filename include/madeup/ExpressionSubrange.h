#ifndef EXPRESSIONSUBRANGE_H
#define EXPRESSIONSUBRANGE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionString.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSubrange : public Expression {
  public:
    ExpressionSubrange(td::Co<Expression> base_expression,
                       td::Co<Expression> min_expression,
                       td::Co<Expression> max_expression);

    td::Co<Expression> evaluate(Environment &env) const;
    td::Co<Expression> evaluateBase(Environment &env) const;
    td::Co<ExpressionInteger> evaluateIndex(Environment &env, int i) const;
    td::Co<ExpressionInteger> evaluateIndex(Environment &env, int i, const ExpressionString *string_value) const;
    td::Co<ExpressionInteger> evaluateIndex(Environment &env, int i, const ExpressionArrayReference *array_value) const;
    void write(ostream &out) const;

    td::Co<Expression> getBase() const;
    td::Co<Expression> getIndex(int i) const;

  private:
    td::Co<Expression> base_expression;
    td::Co<Expression> index_expressions[2];
};

/* ------------------------------------------------------------------------- */

}

#endif
