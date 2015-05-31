#ifndef ELEMENTECHO_H
#define ELEMENTECHO_H

#include "twodee/MessagedException.h"

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionReal.h"
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

    Co<Expression> evaluate(Environment& env) const {
      env.echo();
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(echo)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
