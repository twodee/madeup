#ifndef ELEMENTROTATE_H
#define ELEMENTROTATE_H

#include "twodee/MessagedException.h"

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRotate : public Expression {
  public:
    ExpressionRotate() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> xyz[3] = {
        env["x"],
        env["y"],
        env["z"],
      };
      float v[3];

      if (xyz[0].IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function rotate expects a value named x. No value named x is defined.");
      }

      if (xyz[1].IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function rotate expects a value named y. No value named y is defined.");
      }

      if (xyz[2].IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function rotate expects a value named z. No value named z is defined.");
      }

      Co<ExpressionClosure> degrees_closure = env["degrees"];
      if (degrees_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function rotate expects a value named degrees. No value named degrees is defined.");
      }

      for (int i = 0; i < 3; ++i) {
        Co<Expression> vv = xyz[i]->Evaluate(env);
        ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(vv.GetPointer());
        if (!number) {
          std::stringstream ss;
          ss << xyz[i]->GetSourceLocation() << ": Function rotate expects a numeric " << (char) ('x' + i) << " coordinate. " << xyz[i]->GetSource() << " is not a number.";
          throw MessagedException(ss.str());
        }
        v[i] = number->GetReal();
      }

      Co<Expression> vv = degrees_closure->Evaluate(env);
      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(vv.GetPointer());
      if (!number) {
        std::stringstream ss;
        ss << degrees_closure->GetSourceLocation() << ": Function rotate expects degrees to be a number. " << degrees_closure->GetSource() << " is not a number.";
        throw MessagedException(ss.str());
      }
      float degrees = number->GetReal();

      env.Rotate(v[0], v[1], v[2], degrees);

      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(rotate x y z)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
