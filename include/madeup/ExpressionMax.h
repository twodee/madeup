#ifndef EXPRESSIONMAX_H
#define EXPRESSIONMAX_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMax : public Expression {
  public:
    ExpressionMax() :
      Expression() {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> a_closure = env["a"];
      Co<ExpressionClosure> b_closure = env["b"];

      if (a_closure.IsNull() || b_closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": I expect function max to be given two numbers, a and b. You didn't give max two numbers.");
      }

      Co<Expression> a_value = a_closure->evaluate(env);
      Co<Expression> b_value = b_closure->evaluate(env);

      ExpressionInteger *a_integer = dynamic_cast<ExpressionInteger *>(a_value.GetPointer());
      ExpressionInteger *b_integer = dynamic_cast<ExpressionInteger *>(b_value.GetPointer());
      
      if (a_integer && b_integer) {
        int a = a_integer->toInteger();
        int b = b_integer->toInteger();
        return Co<Expression>(new ExpressionInteger(a > b ? a : b));
      } else {
        ExpressionNumber *a_number = dynamic_cast<ExpressionNumber *>(a_value.GetPointer());
        ExpressionNumber *b_number = dynamic_cast<ExpressionNumber *>(b_value.GetPointer());
        if (!a_number) {
          throw MessagedException(a_closure->getSourceLocation().toAnchor() + ": I expect function max to be given a number. That thing you gave it wasn't a number.");
        } else if (!b_number) {
          throw MessagedException(b_closure->getSourceLocation().toAnchor() + ": I expect function max to be given a number. That thing you gave it wasn't a number.");
        } else {
          float a = a_integer->toReal();
          float b = b_integer->toReal();
          return Co<Expression>(new ExpressionReal(a > b ? a : b));
        }
      }
    }

    void write(ostream& out) const {
      out << "(max a b)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
