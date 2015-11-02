#ifndef ELEMENTDEFINEELEMENT_H
#define ELEMENTDEFINEELEMENT_H

#include "madeup/Expression.h"
#include "madeup/ExpressionSubscript.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDefineElement : public Expression {
  public:
    ExpressionDefineElement(Co<Expression> base,
                            Co<Expression> index,
                            Co<Expression> rhs);
    Co<Expression> evaluate(Environment &env) const;
    Co<Expression> getRightHandSide() const;
    void write(ostream &out) const;

  private:
    Co<ExpressionSubscript> subscript;
    Co<Expression> rhs;
};

/* ------------------------------------------------------------------------- */

}

#endif
