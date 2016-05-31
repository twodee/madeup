#ifndef EXPRESSIONPATH_H
#define EXPRESSIONPATH_H

#include "madeup/Expression.h"
#include "twodee/Trimesh.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPath : public Expression {
  public:
    ExpressionPath();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
