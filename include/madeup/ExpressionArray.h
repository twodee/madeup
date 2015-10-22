#ifndef EXPRESSIONARRAY_H
#define EXPRESSIONARRAY_H

#include "madeup/Expression.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionArray : public Expression {
  public:
    ExpressionArray(int nelements, Co<Expression> fill = ExpressionUnit::getSingleton());

    Co<Expression> evaluate(Environment &env) const;
    int getSize();
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
    Co<ExpressionArray> getArray();
    Co<const ExpressionArray> getArray() const;

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

class ExpressionArrayLiteral : public Expression {
  public:
    ExpressionArrayLiteral(const std::vector<Co<Expression> >& items);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    std::vector<Co<Expression> > items;
};

/* ------------------------------------------------------------------------- */

class ExpressionArraySize : public Expression {
  public:
    ExpressionArraySize();

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
