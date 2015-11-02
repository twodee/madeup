#ifndef EXPRESSIONSUBSCRIPT_H
#define EXPRESSIONSUBSCRIPT_H

#include "madeup/Expression.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionString.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSubscript : public Expression {
  public:
    ExpressionSubscript(Co<Expression> base_expression,
                        Co<Expression> index_expression);

    Co<Expression> evaluate(Environment &env) const;
    Co<Expression> evaluateBase(Environment &env) const;
    Co<ExpressionInteger> evaluateIndex(Environment &env) const;
    Co<ExpressionInteger> evaluateIndex(Environment &env, const ExpressionString *string_value) const;
    Co<ExpressionInteger> evaluateIndex(Environment &env, const ExpressionArrayReference *array_value) const;
    void write(ostream &out) const;

    Co<Expression> getBase() const;
    Co<Expression> getIndex() const;

  private:
    Co<Expression> base_expression;
    Co<Expression> index_expression;
};

/* ------------------------------------------------------------------------- */

}

#endif
