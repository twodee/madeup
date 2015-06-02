#ifndef EXPRESSIONBLOCK_H
#define EXPRESSIONBLOCK_H

#include <vector>

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBlock : public Expression {
  public:
    ExpressionBlock();

    void append(Co<Expression> element);
    Co<Expression> evaluate(Environment &env) const;
    Co<Expression> operator[](int i);
    int getLength() const;
    void write(ostream &out) const;

  private:
    std::vector<Co<Expression> > statements;
};

/* ------------------------------------------------------------------------- */

}

#endif
