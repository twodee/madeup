#ifndef ELEMENTWHERE_H
#define ELEMENTWHERE_H

#include <iostream>

#include "twodee/MessagedException.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionInteger.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionWhere : public Expression {
  public:
    ExpressionWhere() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      std::cerr << env.getTurtle().position << std::endl;
      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(where)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
