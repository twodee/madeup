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

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> min = env["min"];
      Co<ExpressionClosure> max = env["max"];
      if (min.IsNull()) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": Function random expects a value named min. No value named min is defined.");
      }
      if (max.IsNull()) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": Function random expects a value named max. No value named max is defined.");
      }
      Co<Expression> vmin = min->Evaluate(env);
      Co<Expression> vmax = max->Evaluate(env);

      ExpressionInteger *vimin = dynamic_cast<ExpressionInteger *>(vmin.GetPointer());
      ExpressionInteger *vimax = dynamic_cast<ExpressionInteger *>(vmax.GetPointer());

      if (!vimin) {
        throw MessagedException(min->GetSourceLocation().toAnchor() + ": Function random expects its lower bound to be an integer. " + min->GetSource() + " is not an integer.");
      }

      if (!vimax) {
        throw MessagedException(max->GetSourceLocation().toAnchor() + ": Function random expects its upper bound to be an integer. " + max->GetSource() + " is not an integer.");
      }

      int imin = vimin->GetInteger();
      int imax = vimax->GetInteger();

      float random_weight = rand() / (float) RAND_MAX;
      int r = (int) ((imax - imin + 1) * random_weight) + imin;

      return Co<Expression>(new ExpressionInteger(r));
    }

    void Write(ostream& out) const {
      out << "(random min max)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
