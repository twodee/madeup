#ifndef EXPRESSIONABSOLUTEVALUE_H
#define EXPRESSIONABSOLUTEVALUE_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAbsoluteValue : public Expression {
  public:
    ExpressionAbsoluteValue(Co<Expression> expr) :
      Expression(),
      expr(expr) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> value = expr->Evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(value.GetPointer());

      if (!number) {
        throw MessagedException(expr->GetSourceLocation().toAnchor() + ": I can only compute the absolute value of a number. " + expr->GetSource() + " is not a number.");
      }

      ExpressionInteger *integer = dynamic_cast<ExpressionInteger *>(value.GetPointer());
      
      Co<Expression> r;
      if (integer) {
        r = Co<Expression>(new ExpressionInteger(abs(integer->GetInteger())));
      } else {
        r = Co<Expression>(new ExpressionReal(fabs(number->GetReal())));
      }

      return r;
    }

    void Write(ostream& out) const {
      out << "(abs ";
      expr->Write(out);
      out << ")";
    }

  private:
    Co<Expression> expr;
};

/* ------------------------------------------------------------------------- */

}

#endif
