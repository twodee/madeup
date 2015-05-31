#ifndef EXPRESSIONGREATER_H
#define EXPRESSIONGREATER_H

#include <sstream>

#include "madeup/Expression.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionGreater : public Expression {
  public:
    ExpressionGreater(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> lvalue = left->evaluate(env);
      Co<Expression> rvalue = right->evaluate(env);

      ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
      ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());
      ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
      
      // Both are decimals.
      Co<Expression> r;
      if (linteger && rinteger) {
        r = Co<Expression>(new ExpressionBoolean(linteger->toInteger() > rinteger->toInteger()));
      } else if (lnumber && rnumber) {
        r = Co<Expression>(new ExpressionBoolean(lnumber->toReal() > rnumber->toReal()));
      } else {
        throw MessagedException(getSourceLocation().toAnchor() + ": Operator > doesn't know how to relate " + left->getSource() + " and " + right->getSource() + ".");
      }

      return r;
    }

    void write(ostream& out) const {
      out << "(> ";
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
