#ifndef EXPRESSIONSUBSCRIPT_H
#define EXPRESSIONSUBSCRIPT_H

#include "madeup/Expression.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionString.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSubscript : public Expression {
  public:
    ExpressionSubscript(td::Co<Expression> base_expression,
                        td::Co<Expression> index_expression);

    td::Co<Expression> evaluate(Environment &env) const;
    td::Co<Expression> evaluateBase(Environment &env) const;
    td::Co<ExpressionInteger> evaluateIndex(Environment &env) const;
    td::Co<ExpressionInteger> evaluateIndex(Environment &env, const ExpressionString *string_value) const;
    td::Co<ExpressionInteger> evaluateIndex(Environment &env, const ExpressionArrayReference *array_value) const;
    void write(ostream &out) const;

    td::Co<Expression> getBase() const;
    td::Co<Expression> getIndex() const;

  private:
    td::Co<Expression> base_expression;
    td::Co<Expression> index_expression;
};

/* ------------------------------------------------------------------------- */

}

#endif
