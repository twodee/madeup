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

    Co<Expression> Evaluate(Environment& env) {
      env.Dowel();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(dowel)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
