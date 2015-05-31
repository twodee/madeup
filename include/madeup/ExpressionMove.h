#ifndef ELEMENTMOVE_H
#define ELEMENTMOVE_H

#include "twodee/MessagedException.h"

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMove : public Expression {
  public:
    ExpressionMove() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> distance = env["length"];
      if (distance.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function move expects a value named length. No value named length is defined.");
      }
      Co<Expression> v = distance->evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (number) {
        float value = number->toReal();
        env.move(value);
        return v;
      } else {
        throw MessagedException(distance->getSourceLocation().toAnchor() + ": Function move expects a numeric length. " + distance->getSource() + " is not a number.");
      }
    }

    void write(ostream& out) const {
      out << "(move x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
