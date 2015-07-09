#ifndef ELEMENTREPEATWICH_H
#define ELEMENTREPEATWICH_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRepeatwich : public Expression {
  public:
    ExpressionRepeatwich(Co<Expression> nmores, Co<Expression> more, Co<Expression> less);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> nmores;
    Co<Expression> more;
    Co<Expression> less;
};

/* ------------------------------------------------------------------------- */

}

#endif
