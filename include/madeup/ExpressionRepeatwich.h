#ifndef ELEMENTREPEATWICH_H
#define ELEMENTREPEATWICH_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRepeatwich : public Expression {
  public:
    ExpressionRepeatwich(td::Co<Expression> nmores, td::Co<Expression> more, td::Co<Expression> less);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    td::Co<Expression> nmores;
    td::Co<Expression> more;
    td::Co<Expression> less;
};

/* ------------------------------------------------------------------------- */

}

#endif
