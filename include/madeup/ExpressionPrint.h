#ifndef ELEMENTPRINT_H
#define ELEMENTPRINT_H

#include <iostream>

#include "twodee/MessagedException.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionInteger.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionPrint : public Expression {
  public:
    ExpressionPrint() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> message = env["message"];
      if (message.IsNull()) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": I expect function print to be given a value named message, but no message was given.");
      }

      Co<Expression> value = message->Evaluate(env);

      ExpressionString *string_value = dynamic_cast<ExpressionString *>(value.GetPointer());
      if (string_value) {
        std::cout << string_value->GetString() << std::endl;
        return value;
      }

      ExpressionReal *decimal_value = dynamic_cast<ExpressionReal *>(value.GetPointer());
      if (decimal_value) {
        std::cout << decimal_value->GetReal() << std::endl;
        return value;
      }

      ExpressionInteger *integer_value = dynamic_cast<ExpressionInteger *>(value.GetPointer());
      if (integer_value) {
        std::cout << integer_value->GetInteger() << std::endl;
        return value;
      }

      ExpressionBoolean *boolean_value = dynamic_cast<ExpressionBoolean *>(value.GetPointer());
      if (boolean_value) {
        std::cout << boolean_value->GetBoolean() << std::endl;
        return value;
      }
      
      throw MessagedException(message->GetSourceLocation().toAnchor() + ": I expect function print to be given a string, boolean, or number. You gave it something I don't know how to print.");
    }

    void Write(ostream& out) const {
      out << "(print msg)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
