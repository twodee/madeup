#ifndef EXPRESSIONCENTER_H
#define EXPRESSIONCENTER_H

#include "twodee/MessagedException.h"

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionReal.h"
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

    Co<Expression> evaluate(Environment& env) const {
      env.center();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(center)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
