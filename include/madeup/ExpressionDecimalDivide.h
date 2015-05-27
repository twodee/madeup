#ifndef EXPRESSIONDOUBLEDIVIDE_H
#define EXPRESSIONDOUBLEDIVIDE_H

#include <sstream>

#include "madeup/ExpressionNumber.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDecimalDivide : public Expression {
  public:
    ExpressionDecimalDivide(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);
      Co<Expression> rvalue = right->Evaluate(env);

      ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
      ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());

      Co<Expression> value;
      
      // Make sure both are numbers.
      if (lnumber && rnumber) {
        if (rnumber->GetDecimal() != 0.0) {
          value = Co<Expression>(new ExpressionDecimal(lnumber->GetDecimal() / rnumber->GetDecimal()));
        } else {
          throw MessagedException(right->GetSourceLocation() + ": I don't know how to divide by 0.");
        }
      } else {
        throw MessagedException(GetSourceLocation() + ": Operator // doesn't know how to divide " + left->GetSource() + " by " + right->GetSource() + ".");
      }

      return value;
    }

    void Write(ostream& out) const {
      out << "(// ";
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
