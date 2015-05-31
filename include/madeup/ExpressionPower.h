#ifndef EXPRESSIONPOWER_H
#define EXPRESSIONPOWER_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPower : public Expression {
  public:
    ExpressionPower(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> lvalue = left->evaluate(env);
      Co<Expression> rvalue = right->evaluate(env);

      ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
      ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());

      if (!lnumber) {
        throw MessagedException(left->getSourceLocation().toAnchor() + ": Operator ^ expects a numeric base. " + left->getSource() + " is not a number.");
      }

      if (!rnumber) {
        throw MessagedException(right->getSourceLocation().toAnchor() + ": Operator ^ expects a numeric exponent. " + right->getSource() + " is not a number.");
      }

      ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
      
      Co<Expression> r;
      if (linteger && rinteger) {
        r = Co<Expression>(new ExpressionInteger((int) pow(linteger->toInteger(), rinteger->toInteger())));
      } else {
        r = Co<Expression>(new ExpressionReal(pow(lnumber->toReal(), rnumber->toReal())));
      }

      return r;
    }

    void write(ostream& out) const {
      out << "(^ ";
      left->write(out);
      out << " ";
      right->write(out);
      out << ")";
    }

  private:
    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
