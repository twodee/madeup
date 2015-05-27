#ifndef ELEMENTDEBUG_H
#define ELEMENTDEBUG_H

#include <iostream>

#include "twodee/MessagedException.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionDecimal.h"
#include "madeup/ExpressionInteger.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDebug : public Expression {
  public:
    ExpressionDebug() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> message = env["message"];

      if (message.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": I expect function debug to be given a value named message, but no message was given.");
      }

      Co<Expression> value = message->Evaluate(env);

      ExpressionString *string_value = dynamic_cast<ExpressionString *>(value.GetPointer());
      if (string_value) {
        std::cerr << message->GetSource() << ": " << string_value->GetString() << std::endl;
        return value;
      }

      ExpressionDecimal *decimal_value = dynamic_cast<ExpressionDecimal *>(value.GetPointer());
      if (decimal_value) {
        std::cerr << message->GetSource() << ": " << decimal_value->GetDecimal() << std::endl;
        return value;
      }

      ExpressionInteger *integer_value = dynamic_cast<ExpressionInteger *>(value.GetPointer());
      if (integer_value) {
        std::cerr << message->GetSource() << ": " << integer_value->GetInteger() << std::endl;
        return value;
      }

      ExpressionBoolean *boolean_value = dynamic_cast<ExpressionBoolean *>(value.GetPointer());
      if (boolean_value) {
        std::cerr << message->GetSource() << ": " << boolean_value->GetBoolean() << std::endl;
        return value;
      }
      
      throw MessagedException(message->GetSourceLocation() + ": I expect function debug to be given a string, boolean, or number. You gave it something I don't know how to print.");
    }

    void Write(ostream& out) const {
      out << "(debug msg)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
