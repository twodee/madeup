#ifndef EXPRESSIONSUBRANGE_H
#define EXPRESSIONSUBRANGE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionString.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSubrange : public Expression {
  public:
    ExpressionSubrange(Co<Expression> base_expression,
                       Co<Expression> min_expression,
                       Co<Expression> max_expression);

    Co<Expression> evaluate(Environment &env) const;
    Co<Expression> evaluateBase(Environment &env) const;
    Co<ExpressionInteger> evaluateIndex(Environment &env, int i) const;
    Co<ExpressionInteger> evaluateIndex(Environment &env, int i, const ExpressionString *string_value) const;
    Co<ExpressionInteger> evaluateIndex(Environment &env, int i, const ExpressionArrayReference *array_value) const;
    void write(ostream &out) const;

    Co<Expression> getBase() const;
    Co<Expression> getIndex(int i) const;

  private:
    Co<Expression> base_expression;
    Co<Expression> index_expressions[2];
};

/* ------------------------------------------------------------------------- */

}

#endif
