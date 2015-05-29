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

    Co<Expression> Evaluate(Environment& env) {
      env.Identity();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(identity)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
