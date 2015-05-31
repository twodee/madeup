#ifndef ELEMENTBOX_H
#define ELEMENTBOX_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBox : public Expression {
  public:
    ExpressionBox() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      env.box();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(box)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
