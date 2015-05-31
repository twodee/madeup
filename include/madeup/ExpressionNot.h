#ifndef EXPRESSIONNOT_H
#define EXPRESSIONNOT_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionBoolean.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNot : public Expression {
  public:
    ExpressionNot(Co<Expression> left) :
      Expression(),
      left(left) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);

      ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
      
      if (lboolean) {
        return Co<Expression>(new ExpressionBoolean(!lboolean->GetBoolean()));
      } else {
        throw MessagedException(left->GetSourceLocation().toAnchor() + ": Operator not expects a boolean operand. " + left->GetSource() + " is not boolean.");
      }
    }

    void Write(ostream& out) const {
      out << "(not ";
      left->Write(out);
      out << ")";
    }

  private:
    Co<Expression> left;
};

/* ------------------------------------------------------------------------- */

}

#endif
