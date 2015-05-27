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

    Co<Expression> Evaluate(Environment& env) {
      env.Box();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(box)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
