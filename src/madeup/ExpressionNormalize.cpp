#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNormalize.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionString.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionNormalize::ExpressionNormalize() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNormalize::evaluate(Environment &env) const {
  Co<ExpressionClosure> list_closure = env["list"];
  if (list_closure.IsNull()) {
    throw UnlocatedException("I expect function normalize to be given a value named list. No value named list is defined.");
  }

  Co<Expression> list_value = list_closure->evaluate(env);
  ExpressionArrayReference *list_array_value = dynamic_cast<ExpressionArrayReference *>(list_value.GetPointer());
  if (!list_array_value) {
    throw UnlocatedException("I expect function normalize's parameter list to be an array. " + list_closure->getSource() + " is not an array.");
  }

  float dot = 0.0;
  for (int i = 0; i < list_array_value->getArray()->getSize(); ++i) {
    Co<Expression> element = list_array_value->getArray()->operator[](i);
    ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(element.GetPointer());
    if (!number) {
      std::stringstream ss;
      ss << "I expect function normalize's parameter list to be composed of numbers. But element " << i << " is not a number.";
      throw UnlocatedException(ss.str());
    }

    dot += number->toReal() * number->toReal();
  }
  float length = sqrtf(dot);

  Co<ExpressionArray> array(new ExpressionArray(list_array_value->getArray()->getSize()));

  for (int i = 0; i < list_array_value->getArray()->getSize(); ++i) {
    Co<Expression> element = list_array_value->getArray()->operator[](i);
    ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(element.GetPointer());
    array->setElement(i, Co<Expression>(new ExpressionReal(number->toReal() / length)));
  }

  return Co<Expression>(new ExpressionArrayReference(array));
}

/* ------------------------------------------------------------------------- */

void ExpressionNormalize::write(ostream &out) const {
  out << "(normalize array)";
}

/* ------------------------------------------------------------------------- */

}
