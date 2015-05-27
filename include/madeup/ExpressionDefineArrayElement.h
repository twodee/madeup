#ifndef ELEMENTDEFINEARRAYELEMENT_H
#define ELEMENTDEFINEARRAYELEMENT_H

#include "madeup/Expression.h"
#include "madeup/ExpressionArray.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDefineArrayElement : public Expression {
  public:
    ExpressionDefineArrayElement(Co<Expression> array, Co<Expression> index, Co<Expression> rhs);
    Co<Expression> Evaluate(Environment& env);
    Co<Expression> GetRightHandSide() const;
    void Write(ostream& out) const;

  private:
    Co<ExpressionArraySubscript> subscript;
    Co<Expression> rhs;
};

/* ------------------------------------------------------------------------- */

}

#endif
