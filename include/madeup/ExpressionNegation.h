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

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);

      ExpressionInteger *lint = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      if (lint) {
        return Co<Expression>(new ExpressionInteger(-lint->GetInteger()));
      }
      
      ExpressionReal *ldecimal = dynamic_cast<ExpressionReal *>(lvalue.GetPointer());
      if (ldecimal) {
        return Co<Expression>(new ExpressionReal(-ldecimal->GetReal()));
      }

      throw MessagedException(left->GetSourceLocation().toAnchor() + ": Operator not expects a numeric operand. " + left->GetSource() + " is not numeric.");
    }

    void Write(ostream& out) const {
      out << "(negate ";
      left->Write(out);
      out << ")";
    }

  private:
    Co<Expression> left;
};

/* ------------------------------------------------------------------------- */

}

#endif
