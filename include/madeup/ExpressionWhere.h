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

    Co<Expression> Evaluate(Environment& env) {
      std::cerr << env.GetTurtle().position << std::endl;
      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(where)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
