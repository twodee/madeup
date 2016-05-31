#ifndef EXPRESSIONPOLYGON_H
#define EXPRESSIONPOLYGON_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPolygon : public Expression {
  public:
    ExpressionPolygon();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}


#endif
