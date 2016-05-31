#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionSize.h"
#include "madeup/ExpressionString.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionSize::ExpressionSize() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSize::evaluate(Environment &env) const {
  Co<ExpressionClosure> list_closure = env["list"];
  if (list_closure.IsNull()) {
    throw UnlocatedException("I expect function size to be given a value named list. No value named list is defined.");
  }

  Co<Expression> list_value = list_closure->evaluate(env);

  ExpressionArrayReference *array_value = dynamic_cast<ExpressionArrayReference *>(list_value.GetPointer());
  if (array_value) {
    return Co<Expression>(new ExpressionInteger(array_value->getArray()->getSize()));
  }

  ExpressionString *string_value = dynamic_cast<ExpressionString *>(list_value.GetPointer());
  if (string_value) {
    return Co<Expression>(new ExpressionInteger(string_value->getString().length()));
  }

  throw UnlocatedException("I expect function size to be applied to an array or a string. " + list_closure->getSource() + " is not an array.");
}

/* ------------------------------------------------------------------------- */

void ExpressionSize::write(ostream &out) const {
  out << "(size array)";
}

/* ------------------------------------------------------------------------- */

}
