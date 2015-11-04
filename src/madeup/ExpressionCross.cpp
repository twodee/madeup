#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionCross.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionString.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionCross::ExpressionCross() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionCross::evaluate(Environment &env) const {
  Co<ExpressionClosure> a_closure = env["a"];
  if (a_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function cross to be given a value named a. No value named a is defined.");
  }

  Co<ExpressionClosure> b_closure = env["b"];
  if (b_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function cross to be given a value named b. No value named b is defined.");
  }

  Co<Expression> a_value = a_closure->evaluate(env);
  ExpressionArrayReference *a_array_value = dynamic_cast<ExpressionArrayReference *>(a_value.GetPointer());
  if (!a_array_value) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function cross's parameter a to be an array. " + a_closure->getSource() + " is not an array.");
  }

  Co<Expression> b_value = b_closure->evaluate(env);
  ExpressionArrayReference *b_array_value = dynamic_cast<ExpressionArrayReference *>(b_value.GetPointer());
  if (!b_array_value) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function cross's parameter b to be an array. " + b_closure->getSource() + " is not an array.");
  }

  if (a_array_value->getArray()->getSize() != 3) {
    std::stringstream ss;
    ss << getSourceLocation().toAnchor();
    ss << ": I expect function cross's parameter a to have size 3. ";
    ss << "But a has size " << a_array_value->getArray()->getSize() << ".";
    throw MessagedException(ss.str());
  }

  if (b_array_value->getArray()->getSize() != 3) {
    std::stringstream ss;
    ss << getSourceLocation().toAnchor();
    ss << ": I expect function cross's parameter b to have size 3. ";
    ss << "But b has size " << b_array_value->getArray()->getSize() << ".";
    throw MessagedException(ss.str());
  }

  float a[3];
  float b[3];
  for (int i = 0; i < 3; ++i) {
    Co<Expression> element_a = a_array_value->getArray()->operator[](i);
    ExpressionNumber *number_a = dynamic_cast<ExpressionNumber *>(element_a.GetPointer());
    if (!number_a) {
      std::stringstream ss;
      ss << getSourceLocation().toAnchor();
      ss << ": I expect function cross's parameter a to be composed of numbers. But element " << i << " is not a number.";
      throw MessagedException(ss.str());
    }
    a[i] = number_a->toReal();

    Co<Expression> element_b = b_array_value->getArray()->operator[](i);
    ExpressionNumber *number_b = dynamic_cast<ExpressionNumber *>(element_b.GetPointer());
    if (!number_b) {
      std::stringstream ss;
      ss << getSourceLocation().toAnchor();
      ss << ": I expect function cross's parameter b to be composed of numbers. But element " << i << " is not a number.";
      throw MessagedException(ss.str());
    }
    b[i] = number_b->toReal();
  }

  Co<ExpressionArray> array(new ExpressionArray(3));
  array->setElement(0, Co<Expression>(new ExpressionReal(a[1] * b[2] - a[2] * b[1])));
  array->setElement(1, Co<Expression>(new ExpressionReal(a[2] * b[0] - a[0] * b[2])));
  array->setElement(2, Co<Expression>(new ExpressionReal(a[0] * b[1] - a[1] * b[0])));
  return Co<Expression>(new ExpressionArrayReference(array));
}

/* ------------------------------------------------------------------------- */

void ExpressionCross::write(ostream &out) const {
  out << "(cross a b)";
}

/* ------------------------------------------------------------------------- */

}
