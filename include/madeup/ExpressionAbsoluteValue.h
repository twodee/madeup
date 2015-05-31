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

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> value = expr->evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(value.GetPointer());

      if (!number) {
        throw MessagedException(expr->getSourceLocation().toAnchor() + ": I can only compute the absolute value of a number. " + expr->getSource() + " is not a number.");
      }

      ExpressionInteger *integer = dynamic_cast<ExpressionInteger *>(value.GetPointer());
      
      Co<Expression> r;
      if (integer) {
        r = Co<Expression>(new ExpressionInteger(abs(integer->toInteger())));
      } else {
        r = Co<Expression>(new ExpressionReal(fabs(number->toReal())));
      }

      return r;
    }

    void write(ostream& out) const {
      out << "(abs ";
      expr->write(out);
      out << ")";
    }

  private:
    Co<Expression> expr;
};

/* ------------------------------------------------------------------------- */

}

#endif
