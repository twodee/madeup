#ifndef EXPRESSIONARRAY_H
#define EXPRESSIONARRAY_H

#include "madeup/Expression.h"
#include "madeup/ExpressionInteger.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionArray : public Expression {
  public:
    ExpressionArray(int nelements, Co<Expression> fill);

    Co<Expression> evaluate(Environment &env) const;
    int getLength();
    Co<Expression> operator[](int i);
    void setElement(int i, Co<Expression> expr);
    void write(ostream &out) const;

  private:
    Co<Expression> *elements;
    int nelements;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayReference : public Expression {
  public:
    ExpressionArrayReference(Co<ExpressionArray> array_expression);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;
    Co<ExpressionArray> GetArray();

  private:
    Co<ExpressionArray> array_expression;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayConstructor : public Expression {
  public:
    ExpressionArrayConstructor(Co<Expression> fill_expression, Co<Expression> length_expression);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> fill_expression;
    Co<Expression> length_expression;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayLength : public Expression {
  public:
    ExpressionArrayLength(Co<Expression> array_expression);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> array_expression;
};

/* ------------------------------------------------------------------------- */

class ExpressionArraySubscript : public Expression {
  public:
    ExpressionArraySubscript(Co<Expression> array_expression, Co<Expression> index_expression);

    Co<ExpressionArrayReference> evaluateArrayReference(Environment &env) const;
    Co<ExpressionInteger> evaluateIndex(Environment &env, Co<ExpressionArrayReference> array) const;
    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> array_expression;
    Co<Expression> index_expression;
};

/* ------------------------------------------------------------------------- */

}

#endif
