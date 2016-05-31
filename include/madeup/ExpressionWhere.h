#ifndef ELEMENTWHERE_H
#define ELEMENTWHERE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionWhere : public Expression {
  public:
    ExpressionWhere();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

class ExpressionForward : public Expression {
  public:
    ExpressionForward();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

class ExpressionRight : public Expression {
  public:
    ExpressionRight();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

class ExpressionUp : public Expression {
  public:
    ExpressionUp();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
