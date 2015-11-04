#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionMagnitude.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionString.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionMagnitude::ExpressionMagnitude() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionMagnitude::evaluate(Environment &env) const {
  Co<ExpressionClosure> list_closure = env["list"];
  if (list_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function magnitude to be given a value named list. No value named list is defined.");
  }

  Co<Expression> list_value = list_closure->evaluate(env);
  ExpressionArrayReference *list_array_value = dynamic_cast<ExpressionArrayReference *>(list_value.GetPointer());
  if (!list_array_value) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function magnitude's parameter list to be an array. " + list_closure->getSource() + " is not an array.");
  }

  float dot = 0.0;
  for (int i = 0; i < list_array_value->getArray()->getSize(); ++i) {
    Co<Expression> element = list_array_value->getArray()->operator[](i);
    ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(element.GetPointer());
    if (!number) {
      std::stringstream ss;
      ss << getSourceLocation().toAnchor();
      ss << ": I expect function magnitude's parameter list to be composed of numbers. But element " << i << " is not a number.";
      throw MessagedException(ss.str());
    }

    dot += number->toReal() * number->toReal();
  }
  float magnitude = sqrtf(dot);

  return Co<Expression>(new ExpressionReal(magnitude));
}

/* ------------------------------------------------------------------------- */

void ExpressionMagnitude::write(ostream &out) const {
  out << "(magnitude array)";
}

/* ------------------------------------------------------------------------- */

}
