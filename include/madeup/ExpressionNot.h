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

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> lvalue = left->evaluate(env);

      ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
      
      if (lboolean) {
        return Co<Expression>(new ExpressionBoolean(!lboolean->toBoolean()));
      } else {
        throw MessagedException(left->getSourceLocation().toAnchor() + ": Operator not expects a boolean operand. " + left->getSource() + " is not boolean.");
      }
    }

    void write(ostream& out) const {
      out << "(not ";
      left->write(out);
      out << ")";
    }

  private:
    Co<Expression> left;
};

/* ------------------------------------------------------------------------- */

}

#endif
