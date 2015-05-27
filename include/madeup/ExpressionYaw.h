#ifndef ELEMENTYAW_H
#define ELEMENTYAW_H

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionYaw : public Expression {
  public:
    ExpressionYaw() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> degrees_closure = env["degrees"];
      if (degrees_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function yaw expects a value named degrees. No value named degrees is defined.");
      }

      Co<Expression> v = degrees_closure->Evaluate(env);
      ExpressionNumber *degrees = dynamic_cast<ExpressionNumber *>(v.GetPointer());

      if (degrees) {
        float value = degrees->GetDecimal();
        env.Yaw(value);
        return v;
      } else {
        throw MessagedException(degrees_closure->GetSourceLocation() + ": Function yaw expects a number of degrees. " + degrees_closure->GetSource() + " is not a number.");
      }
    }

    void Write(ostream& out) const {
      out << "(yaw x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
