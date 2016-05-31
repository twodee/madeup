#ifndef EXPRESSIONBLOCK_H
#define EXPRESSIONBLOCK_H

#include <vector>

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBlock : public Expression {
  public:
    ExpressionBlock();

    void append(td::Co<Expression> element);
    td::Co<Expression> evaluate(Environment &env) const;
    td::Co<Expression> operator[](size_t i);
    int getLength() const;
    void write(ostream &out) const;

  private:
    std::vector<td::Co<Expression> > statements;
};

/* ------------------------------------------------------------------------- */

}

#endif
