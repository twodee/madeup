#ifndef ELEMENTLOG_H
#define ELEMENTLOG_H

#include <math.h>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionNumber.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionLog : public Expression {
  public:
    ExpressionLog() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> base_closure = env["base"];
      Co<ExpressionClosure> x_closure = env["x"];
      if (base_closure.IsNull() || x_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": I expect log to be given two parameters: a base and a value x whose logarithm you want to compute.");
      }

      Co<Expression> base_value = base_closure->Evaluate(env);
      ExpressionNumber *base_number = dynamic_cast<ExpressionNumber *>(base_value.GetPointer());
      if (!base_number) {
        throw MessagedException(base_closure->GetSourceLocation() + ": I expect log's base to be a number. That thing you gave me sure isn't a number.");
      }

      Co<Expression> x_value = x_closure->Evaluate(env);
      ExpressionNumber *x_number = dynamic_cast<ExpressionNumber *>(x_value.GetPointer());
      if (!x_number) {
        throw MessagedException(x_closure->GetSourceLocation() + ": I expect log's x to be a number. That thing you gave me sure isn't a number.");
      }

      float base = base_number->GetReal();
      float x = x_number->GetReal();
      return Co<Expression>(new ExpressionReal(log(x) / log(base)));
    }

    void Write(ostream& out) const {
      out << "(tan x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
