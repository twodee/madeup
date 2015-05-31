#ifndef EXPRESSIONOR_H
#define EXPRESSIONOR_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionBoolean.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionOr : public Expression {
  public:
    ExpressionOr(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);
      Co<Expression> rvalue = right->Evaluate(env);

      ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
      ExpressionBoolean *rboolean = dynamic_cast<ExpressionBoolean *>(rvalue.GetPointer());
      
      if (lboolean && rboolean) {
        return Co<Expression>(new ExpressionBoolean(lboolean->GetBoolean() || rboolean->GetBoolean()));
      } else if (!lboolean) {
        throw MessagedException(left->GetSourceLocation().toAnchor() + ": Operator or expects boolean operands. " + left->GetSource() + " is not boolean.");
      } else {
        throw MessagedException(right->GetSourceLocation().toAnchor() + ": Operator or expects boolean operands. " + right->GetSource() + " is not boolean.");
      }
    }

    void Write(ostream& out) const {
      out << "(or ";
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
