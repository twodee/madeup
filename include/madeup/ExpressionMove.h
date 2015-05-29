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

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> distance = env["length"];
      if (distance.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function move expects a value named length. No value named length is defined.");
      }
      Co<Expression> v = distance->Evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (number) {
        float value = number->GetReal();
        env.Move(value);
        return v;
      } else {
        throw MessagedException(distance->GetSourceLocation() + ": Function move expects a numeric length. " + distance->GetSource() + " is not a number.");
      }
    }

    void Write(ostream& out) const {
      out << "(move x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
