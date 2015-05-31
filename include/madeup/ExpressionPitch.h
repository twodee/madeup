#ifndef ELEMENTPITCH_H
#define ELEMENTPITCH_H

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPitch : public Expression {
  public:
    ExpressionPitch() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> degrees_closure = env["degrees"];
      if (degrees_closure.IsNull()) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": Function pitch expects a value named degrees. No value named degrees is defined.");
      }

      Co<Expression> v = degrees_closure->Evaluate(env);

      ExpressionNumber *degrees = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (degrees) {
        float value = degrees->GetReal();
        env.Pitch(value);
        return v;
      } else {
        throw MessagedException(degrees_closure->GetSourceLocation().toAnchor() + ": Function pitch expects a number of degrees. " + degrees_closure->GetSource() + " is not a number.");
      }
    }

    void Write(ostream& out) const {
      out << "(pitch x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
