#ifndef EXPRESSIONSIGN_H
#define EXPRESSIONSIGN_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionDecimal.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSign : public Expression {
  public:
    ExpressionSign() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> n_closure = env["n"];
      if (n_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": I expect function sign to be given a number n.");
      }

      Co<Expression> n_value = n_closure->Evaluate(env);

      ExpressionInteger *n_integer = dynamic_cast<ExpressionInteger *>(n_value.GetPointer());
      if (n_integer) {
        int n = n_integer->GetInteger();
        return Co<Expression>(new ExpressionInteger(n > 0 ? 1 : (n < 0 ? -1 : 0)));
      } else {
        ExpressionNumber *n_number = dynamic_cast<ExpressionNumber *>(n_value.GetPointer());
        if (n_number) {
          float n = n_number->GetDecimal();
          return Co<Expression>(new ExpressionInteger(n > 0 ? 1 : (n < 0 ? -1 : 0)));
        } else {
          throw MessagedException(n_closure->GetSourceLocation() + ": I expect function sign to be given a number. That thing you gave it wasn't a number.");
        }
      }
    }

    void Write(ostream& out) const {
      out << "(sign a b)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
