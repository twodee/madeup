#ifndef EXPRESSIONREMAINDER_H
#define EXPRESSIONREMAINDER_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRemainder : public Expression {
  public:
    ExpressionRemainder(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);
      Co<Expression> rvalue = right->Evaluate(env);

      ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
      
      if (linteger && rinteger) {
        if (rinteger->GetInteger() != 0) {
          return Co<Expression>(new ExpressionInteger(linteger->GetInteger() % rinteger->GetInteger()));
        } else {
          throw MessagedException(right->GetSourceLocation() + ": I don't know how to get the remainder of something divided by 0.");
        }
      } else {
        throw MessagedException(GetSourceLocation() + ": Operator % doesn't know how to get the remainder of " + left->GetSource() + " divided by " + right->GetSource() + ".");
      }
    }

    void Write(ostream& out) const {
      out << "(% ";
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
