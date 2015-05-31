#ifndef EXPRESSIONNEGATION_H
#define EXPRESSIONNEGATION_H

#include <sstream>

#include "madeup/Expression.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNegation : public Expression {
  public:
    ExpressionNegation(Co<Expression> left) :
      Expression(),
      left(left) {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> lvalue = left->evaluate(env);

      ExpressionInteger *lint = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      if (lint) {
        return Co<Expression>(new ExpressionInteger(-lint->toInteger()));
      }
      
      ExpressionReal *ldecimal = dynamic_cast<ExpressionReal *>(lvalue.GetPointer());
      if (ldecimal) {
        return Co<Expression>(new ExpressionReal(-ldecimal->toReal()));
      }

      throw MessagedException(left->getSourceLocation().toAnchor() + ": Operator not expects a numeric operand. " + left->getSource() + " is not numeric.");
    }

    void write(ostream& out) const {
      out << "(negate ";
      left->write(out);
      out << ")";
    }

  private:
    Co<Expression> left;
};

/* ------------------------------------------------------------------------- */

}

#endif
