#ifndef ELEMENTFOR_H
#define ELEMENTFOR_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionFor : public Expression {
  public:
    ExpressionFor(const string &id, td::Co<Expression> start, td::Co<Expression> end, td::Co<Expression> delta, td::Co<Expression> body, bool is_inclusive);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    string id;
    td::Co<Expression> start;
    td::Co<Expression> end;
    td::Co<Expression> delta;
    td::Co<Expression> body;
    bool is_inclusive;
};

/* ------------------------------------------------------------------------- */

}

#endif
