#ifndef EXPRESSIONCENTER_H
#define EXPRESSIONCENTER_H

#include "twodee/MessagedException.h"

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDecimal.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCenter : public Expression {
  public:
    ExpressionCenter() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      env.Center();
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(center)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
