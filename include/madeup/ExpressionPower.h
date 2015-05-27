#ifndef EXPRESSIONPOWER_H
#define EXPRESSIONPOWER_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionDecimal.h"
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

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);
      Co<Expression> rvalue = right->Evaluate(env);

      ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
      ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());

      if (!lnumber) {
        throw MessagedException(left->GetSourceLocation() + ": Operator ^ expects a numeric base. " + left->GetSource() + " is not a number.");
      }

      if (!rnumber) {
        throw MessagedException(right->GetSourceLocation() + ": Operator ^ expects a numeric exponent. " + right->GetSource() + " is not a number.");
      }

      ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
      
      Co<Expression> r;
      if (linteger && rinteger) {
        r = Co<Expression>(new ExpressionInteger((int) pow(linteger->GetInteger(), rinteger->GetInteger())));
      } else {
        r = Co<Expression>(new ExpressionDecimal(pow(lnumber->GetDecimal(), rnumber->GetDecimal())));
      }

      return r;
    }

    void Write(ostream& out) const {
      out << "(^ ";
      left->Write(out);
      out << " ";
      right->Write(out);
      out << ")";
    }

  private:
    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
