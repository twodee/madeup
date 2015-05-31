#ifndef ELEMENTDOWEL_H
#define ELEMENTDOWEL_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDowel : public Expression {
  public:
    ExpressionDowel() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      env.dowel();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(dowel)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
