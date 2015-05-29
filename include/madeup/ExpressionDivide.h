#ifndef EXPRESSIONDIVIDE_H
#define EXPRESSIONDIVIDE_H

#include <sstream>

#include "madeup/Expression.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDivide : public Expression {
  public:
    ExpressionDivide(Co<Expression> left, Co<Expression> right) :
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

      Co<Expression> value;
      
      // Both are decimals.
      if (linteger && rinteger) {
        if (rinteger->GetInteger() != 0) {
          value = Co<Expression>(new ExpressionInteger(linteger->GetInteger() / rinteger->GetInteger()));
        } else {
          throw MessagedException(right->GetSourceLocation() + ": I don't know how to divide by 0.");
        }
      } else if (lnumber && rnumber) {
        if (rnumber->GetReal() != 0.0) {
          value = Co<Expression>(new ExpressionReal(lnumber->GetReal() / rnumber->GetReal()));
        } else {
          throw MessagedException(right->GetSourceLocation() + ": I don't know how to divide by 0.");
        }
      } else {
        throw MessagedException(GetSourceLocation() + ": Operator / doesn't know how to divide " + left->GetSource() + " by " + right->GetSource() + ".");
      }

      return value;
    }

    void Write(ostream& out) const {
      out << "(/ ";
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
