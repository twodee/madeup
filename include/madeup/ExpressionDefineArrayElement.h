#ifndef ELEMENTDEFINEARRAYELEMENT_H
#define ELEMENTDEFINEARRAYELEMENT_H

#include "madeup/Expression.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionCall.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDefineArrayElement : public Expression {
  public:
    ExpressionDefineArrayElement(Co<ExpressionCall> subscript, Co<Expression> rhs);
    Co<Expression> evaluate(Environment &env) const;
    Co<Expression> getRightHandSide() const;
    void write(ostream &out) const;

  private:
    Co<ExpressionCall> subscript;
    Co<Expression> rhs;
};

/* ------------------------------------------------------------------------- */

}

#endif
