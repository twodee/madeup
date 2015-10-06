#include <iostream>

#include "madeup/ExpressionArray.h"
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

bool ExpressionPrint::print(const Co<Expression>& value) {
  const ExpressionString *string_value = dynamic_cast<const ExpressionString *>(value.GetPointer());
  if (string_value) {
    std::cout << string_value->getString();
    return true;
  }

  const ExpressionReal *decimal_value = dynamic_cast<const ExpressionReal *>(value.GetPointer());
  if (decimal_value) {
    std::cout << decimal_value->toReal();
    return true;
  }

  const ExpressionInteger *integer_value = dynamic_cast<const ExpressionInteger *>(value.GetPointer());
  if (integer_value) {
    std::cout << integer_value->toInteger();
    return true;
  }

  const ExpressionBoolean *boolean_value = dynamic_cast<const ExpressionBoolean *>(value.GetPointer());
  if (boolean_value) {
    std::cout << (boolean_value->toBoolean() ? "true" : "false");
    return true;
  }

  const ExpressionUnit *unit_value = dynamic_cast<const ExpressionUnit *>(value.GetPointer());
  if (unit_value) {
    std::cout << "nothing";
    return true;
  }

  const ExpressionArrayReference *array_reference = dynamic_cast<const ExpressionArrayReference *>(value.GetPointer());
  if (array_reference) {
    Co<ExpressionArray> array_value = array_reference->getArray();
    std::cout << "{";
    if (array_value->getSize() > 0) {
      print((*array_value)[0]);
      for (int i = 1; i < array_value->getSize(); ++i) {
        std::cout << ", ";
        print((*array_value)[i]);
      }
    }
    std::cout << "}";
    return true;
  }

  return false;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPrint::evaluate(Environment &env) const {
  Co<ExpressionClosure> message = env["message"];
  if (message.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function print to be given a value named message, but no message was given.");
  }

  Co<Expression> value = message->evaluate(env);
  bool isPrintable = print(value);

  if (isPrintable) {
    std::cout << std::endl;
    return value;
  } else {
    throw MessagedException(message->getSourceLocation().toAnchor() + ": I expect function print to be given a string, boolean, number, or array. You gave it something I don't know how to print.");
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionPrint::write(ostream &out) const {
  out << "(print msg)";
}

/* ------------------------------------------------------------------------- */

}
