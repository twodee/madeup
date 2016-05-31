#ifndef ELEMENTREPEAT_H
#define ELEMENTREPEAT_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRepeat : public Expression {
  public:
    ExpressionRepeat(td::Co<Expression> niterations, td::Co<Expression> body);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    td::Co<Expression> niterations;
    td::Co<Expression> body;
};

/* ------------------------------------------------------------------------- */

}

#endif
