#ifndef ELEMENTDEFINEELEMENT_H
#define ELEMENTDEFINEELEMENT_H

#include "madeup/Expression.h"
#include "madeup/ExpressionSubscript.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDefineElement : public Expression {
  public:
    ExpressionDefineElement(td::Co<Expression> base,
                            td::Co<Expression> index,
                            td::Co<Expression> rhs);
    td::Co<Expression> evaluate(Environment &env) const;
    td::Co<Expression> getRightHandSide() const;
    void write(ostream &out) const;

  private:
    td::Co<ExpressionSubscript> subscript;
    td::Co<Expression> rhs;
};

/* ------------------------------------------------------------------------- */

}

#endif
