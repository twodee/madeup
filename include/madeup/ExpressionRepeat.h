#ifndef ELEMENTREPEAT_H
#define ELEMENTREPEAT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRepeat : public Expression {
  public:
    ExpressionRepeat(Co<Expression> niterations, Co<Expression> body);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    Co<Expression> niterations;
    Co<Expression> body;
};

/* ------------------------------------------------------------------------- */

}

#endif
