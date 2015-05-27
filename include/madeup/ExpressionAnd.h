#ifndef EXPRESSIONAND_H
#define EXPRESSIONAND_H

#include <sstream>

#include "madeup/Expression.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAnd : public Expression {
  public:
    ExpressionAnd(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);
      Co<Expression> rvalue = right->Evaluate(env);

      ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
      ExpressionBoolean *rboolean = dynamic_cast<ExpressionBoolean *>(rvalue.GetPointer());
      
      Co<Expression> value;
      if (lboolean && rboolean) {
        value = Co<Expression>(new ExpressionBoolean(lboolean->GetBoolean() && rboolean->GetBoolean()));
      } else if (!lboolean) {
        throw MessagedException(left->GetSourceLocation() + ": Operator and expects boolean operands. " + left->GetSource() + " is not boolean.");
      } else if (!rboolean) {
        throw MessagedException(right->GetSourceLocation() + ": Operator or expects boolean operands. " + right->GetSource() + " is not boolean.");
      }

      return value;
    }

    void Write(ostream& out) const {
      out << "(and ";
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
