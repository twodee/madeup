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

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> lvalue = left->evaluate(env);
      Co<Expression> rvalue = right->evaluate(env);

      ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
      ExpressionBoolean *rboolean = dynamic_cast<ExpressionBoolean *>(rvalue.GetPointer());
      
      if (lboolean && rboolean) {
        return Co<Expression>(new ExpressionBoolean(lboolean->toBoolean() || rboolean->toBoolean()));
      } else if (!lboolean) {
        throw MessagedException(left->getSourceLocation().toAnchor() + ": Operator or expects boolean operands. " + left->getSource() + " is not boolean.");
      } else {
        throw MessagedException(right->getSourceLocation().toAnchor() + ": Operator or expects boolean operands. " + right->getSource() + " is not boolean.");
      }
    }

    void write(ostream& out) const {
      out << "(or ";
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
