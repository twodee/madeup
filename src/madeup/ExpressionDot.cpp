#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDot.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionString.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDot::ExpressionDot() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDot::evaluate(Environment &env) const {
  Co<ExpressionClosure> a_closure = env["a"];
  if (a_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function dot to be given a value named a. No value named a is defined.");
  }

  Co<ExpressionClosure> b_closure = env["b"];
  if (b_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function dot to be given a value named b. No value named b is defined.");
  }

  Co<Expression> a_value = a_closure->evaluate(env);
  ExpressionArrayReference *a_array_value = dynamic_cast<ExpressionArrayReference *>(a_value.GetPointer());
  if (!a_array_value) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function dot's parameter a to be an array. " + a_closure->getSource() + " is not an array.");
  }

  Co<Expression> b_value = b_closure->evaluate(env);
  ExpressionArrayReference *b_array_value = dynamic_cast<ExpressionArrayReference *>(b_value.GetPointer());
  if (!b_array_value) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function dot's parameter b to be an array. " + b_closure->getSource() + " is not an array.");
  }

  if (a_array_value->getArray()->getSize() != b_array_value->getArray()->getSize()) {
    std::stringstream ss;
    ss << getSourceLocation().toAnchor();
    ss << ": I expect function dot's parameters a and b to be the same size. ";
    ss << "But a has size " << a_array_value->getArray()->getSize() << " and ";
    ss << "b has size " << b_array_value->getArray()->getSize() << ".";
    throw MessagedException(ss.str());
  }

  float dot = 0.0;
  for (int i = 0; i < a_array_value->getArray()->getSize(); ++i) {
    Co<Expression> element_a = a_array_value->getArray()->operator[](i);
    ExpressionNumber *number_a = dynamic_cast<ExpressionNumber *>(element_a.GetPointer());
    if (!number_a) {
      std::stringstream ss;
      ss << getSourceLocation().toAnchor();
      ss << ": I expect function dot's parameter a to be composed of numbers. But element " << i << " is not a number.";
      throw MessagedException(ss.str());
    }

    Co<Expression> element_b = b_array_value->getArray()->operator[](i);
    ExpressionNumber *number_b = dynamic_cast<ExpressionNumber *>(element_b.GetPointer());
    if (!number_b) {
      std::stringstream ss;
      ss << getSourceLocation().toAnchor();
      ss << ": I expect function dot's parameter b to be composed of numbers. But element " << i << " is not a number.";
      throw MessagedException(ss.str());
    }

    dot += number_a->toReal() * number_b->toReal();
  }

  return Co<Expression>(new ExpressionReal(dot));
}

/* ------------------------------------------------------------------------- */

void ExpressionDot::write(ostream &out) const {
  out << "(dot array)";
}

/* ------------------------------------------------------------------------- */

}
