#ifndef ELEMENTECHO_H
#define ELEMENTECHO_H

#include "twodee/MessagedException.h"

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDecimal.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionEcho : public Expression {
  public:
    ExpressionEcho() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      env.Echo();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(echo)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
