#ifndef EXPRESSIONARRAY_H
#define EXPRESSIONARRAY_H

#include "madeup/Expression.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionArray : public Expression {
  public:
    ExpressionArray(int nelements, td::Co<Expression> fill = ExpressionUnit::getSingleton());

    td::Co<Expression> evaluate(Environment &env) const;
    int getSize() const;
    td::Co<Expression> operator[](int i);
    void setElement(int i, td::Co<Expression> expr);
    void write(ostream &out) const;

  private:
    td::Co<Expression> *elements;
    int nelements;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayReference : public Expression {
  public:
    ExpressionArrayReference(td::Co<ExpressionArray> array_expression);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;
    td::Co<ExpressionArray> getArray();
    td::Co<const ExpressionArray> getArray() const;

  private:
    td::Co<ExpressionArray> array_expression;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayConstructor : public Expression {
  public:
    ExpressionArrayConstructor(td::Co<Expression> fill_expression, td::Co<Expression> length_expression);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    td::Co<Expression> fill_expression;
    td::Co<Expression> length_expression;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayLiteral : public Expression {
  public:
    ExpressionArrayLiteral(const std::vector<td::Co<Expression> >& items);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    std::vector<td::Co<Expression> > items;
};

/* ------------------------------------------------------------------------- */

}

#endif
