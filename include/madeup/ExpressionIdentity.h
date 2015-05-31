#ifndef ELEMENTIDENTITY_H
#define ELEMENTIDENTITY_H

#include "twodee/MessagedException.h"

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionIdentity : public Expression {
  public:
    ExpressionIdentity() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      env.identity();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(identity)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
