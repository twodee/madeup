#ifndef ELEMENTTRANSLATE_H
#define ELEMENTTRANSLATE_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionTranslate : public Expression {
  public:
    ExpressionTranslate();

    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
