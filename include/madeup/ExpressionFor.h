#ifndef ELEMENTFOR_H
#define ELEMENTFOR_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionFor : public Expression {
  public:
    ExpressionFor(const string &id, Co<Expression> start, Co<Expression> end, Co<Expression> delta, Co<Expression> body, bool is_inclusive);

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    string id;
    Co<Expression> start;
    Co<Expression> end;
    Co<Expression> delta;
    Co<Expression> body;
    bool is_inclusive;
};

/* ------------------------------------------------------------------------- */

}

#endif
