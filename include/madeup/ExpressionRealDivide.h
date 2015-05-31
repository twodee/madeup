#ifndef EXPRESSIONREALDIVIDE_H
#define EXPRESSIONREALDIVIDE_H

#include <sstream>

#include "madeup/ExpressionNumber.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRealDivide : public Expression {
  public:
    ExpressionRealDivide(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> lvalue = left->evaluate(env);
      Co<Expression> rvalue = right->evaluate(env);

      ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
      ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());

      Co<Expression> value;
      
      // Make sure both are numbers.
      if (lnumber && rnumber) {
        if (rnumber->toReal() != 0.0) {
          value = Co<Expression>(new ExpressionReal(lnumber->toReal() / rnumber->toReal()));
        } else {
          throw MessagedException(right->getSourceLocation().toAnchor() + ": I don't know how to divide by 0.");
        }
      } else {
        throw MessagedException(getSourceLocation().toAnchor() + ": Operator // doesn't know how to divide " + left->getSource() + " by " + right->getSource() + ".");
      }

      return value;
    }

    void write(ostream& out) const {
      out << "(// ";
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
