#ifndef EXPRESSIONRANDOM_H
#define EXPRESSIONRANDOM_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionInteger.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRandom : public Expression {
  public:
    ExpressionRandom() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> min = env["min"];
      Co<ExpressionClosure> max = env["max"];
      if (min.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function random expects a value named min. No value named min is defined.");
      }
      if (max.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function random expects a value named max. No value named max is defined.");
      }
      Co<Expression> vmin = min->evaluate(env);
      Co<Expression> vmax = max->evaluate(env);

      ExpressionInteger *vimin = dynamic_cast<ExpressionInteger *>(vmin.GetPointer());
      ExpressionInteger *vimax = dynamic_cast<ExpressionInteger *>(vmax.GetPointer());

      if (!vimin) {
        throw MessagedException(min->getSourceLocation().toAnchor() + ": Function random expects its lower bound to be an integer. " + min->getSource() + " is not an integer.");
      }

      if (!vimax) {
        throw MessagedException(max->getSourceLocation().toAnchor() + ": Function random expects its upper bound to be an integer. " + max->getSource() + " is not an integer.");
      }

      int imin = vimin->toInteger();
      int imax = vimax->toInteger();

      float random_weight = rand() / (float) RAND_MAX;
      int r = (int) ((imax - imin + 1) * random_weight) + imin;

      return Co<Expression>(new ExpressionInteger(r));
    }

    void write(ostream& out) const {
      out << "(random min max)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
