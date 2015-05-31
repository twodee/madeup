#ifndef EXPRESSIONSUBTRACT_H
#define EXPRESSIONSUBTRACT_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSubtract : public Expression {
  public:
    ExpressionSubtract(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);
      Co<Expression> rvalue = right->Evaluate(env);

      ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
      ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());
      ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
      
      // Both are decimals.
      Co<Expression> r;
      if (linteger && rinteger) {
        r = Co<Expression>(new ExpressionInteger(linteger->GetInteger() - rinteger->GetInteger()));
      } else if (lnumber && rnumber) {
        r = Co<Expression>(new ExpressionReal(lnumber->GetReal() - rnumber->GetReal()));
      } else {
        throw MessagedException(GetSourceLocation().toAnchor() + ": Operator - doesn't know how to subtract " + right->GetSource() + " from " + left->GetSource() + ".");
      }

      return r;
    }

    void Write(ostream& out) const {
      out << "(- ";
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
