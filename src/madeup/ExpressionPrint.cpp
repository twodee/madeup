#include <iostream>

#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionPrint.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPrint::ExpressionPrint() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPrint::evaluate(Environment &env) const {
  Co<ExpressionClosure> message = env["message"];
  if (message.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function print to be given a value named message, but no message was given.");
  }

  Co<Expression> value = message->evaluate(env);

  ExpressionString *string_value = dynamic_cast<ExpressionString *>(value.GetPointer());
  if (string_value) {
    std::cout << string_value->getString() << std::endl;
    return value;
  }

  ExpressionReal *decimal_value = dynamic_cast<ExpressionReal *>(value.GetPointer());
  if (decimal_value) {
    std::cout << decimal_value->toReal() << std::endl;
    return value;
  }

  ExpressionInteger *integer_value = dynamic_cast<ExpressionInteger *>(value.GetPointer());
  if (integer_value) {
    std::cout << integer_value->toInteger() << std::endl;
    return value;
  }

  ExpressionBoolean *boolean_value = dynamic_cast<ExpressionBoolean *>(value.GetPointer());
  if (boolean_value) {
    std::cout << (boolean_value->toBoolean() ? "true" : "false") << std::endl;
    return value;
  }

  ExpressionUnit *unit_value = dynamic_cast<ExpressionUnit *>(value.GetPointer());
  if (unit_value) {
    std::cout << "nothing" << std::endl;
    return value;
  }
  
  throw MessagedException(message->getSourceLocation().toAnchor() + ": I expect function print to be given a string, boolean, or number. You gave it something I don't know how to print.");
}

/* ------------------------------------------------------------------------- */

void ExpressionPrint::write(ostream &out) const {
  out << "(print msg)";
}

/* ------------------------------------------------------------------------- */

}
