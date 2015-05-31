#ifndef EXPRESSIONMIN_H
#define EXPRESSIONMIN_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMin : public Expression {
  public:
    ExpressionMin() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> a_closure = env["a"];
      Co<ExpressionClosure> b_closure = env["b"];

      if (a_closure.IsNull() || b_closure.IsNull()) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": I expect function min to be given two numbers, a and b. You didn't give min two numbers.");
      }

      Co<Expression> a_value = a_closure->Evaluate(env);
      Co<Expression> b_value = b_closure->Evaluate(env);

      ExpressionInteger *a_integer = dynamic_cast<ExpressionInteger *>(a_value.GetPointer());
      ExpressionInteger *b_integer = dynamic_cast<ExpressionInteger *>(b_value.GetPointer());
      
      if (a_integer && b_integer) {
        int a = a_integer->GetInteger();
        int b = b_integer->GetInteger();
        return Co<Expression>(new ExpressionInteger(a < b ? a : b));
      } else {
        ExpressionNumber *a_number = dynamic_cast<ExpressionNumber *>(a_value.GetPointer());
        ExpressionNumber *b_number = dynamic_cast<ExpressionNumber *>(b_value.GetPointer());
        if (!a_number) {
          throw MessagedException(a_closure->GetSourceLocation().toAnchor() + ": I expect function min to be given a number. That thing you gave it wasn't a number.");
        } else if (!b_number) {
          throw MessagedException(b_closure->GetSourceLocation().toAnchor() + ": I expect function min to be given a number. That thing you gave it wasn't a number.");
        } else {
          float a = a_integer->GetReal();
          float b = b_integer->GetReal();
          return Co<Expression>(new ExpressionReal(a < b ? a : b));
        }
      }
    }

    void Write(ostream& out) const {
      out << "(min a b)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
