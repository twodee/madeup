#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionArray::ExpressionArray(int nelements, Co<Expression> fill) :
  Expression(),
  nelements(nelements) {
  elements = new Co<Expression>[nelements];
  for (int i = 0; i < nelements; ++i) {
    elements[i] = fill;
  }
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArray::evaluate(Environment &env) const {
  return Co<Expression>((ExpressionArray *) (this));
}

/* ------------------------------------------------------------------------- */

int ExpressionArray::getSize() {
  return nelements;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArray::operator[](int i) {
  if (i < 0 || i >= nelements) {
    throw MessagedException("ack!");
  }
  return elements[i];
}

/* ------------------------------------------------------------------------- */

void ExpressionArray::setElement(int i, Co<Expression> expr) {
  elements[i] = expr;
}

/* ------------------------------------------------------------------------- */

void ExpressionArray::write(ostream &out) const {
  out << "(ARRAY";
  for (int i = 0; i < nelements; ++i) {
    out << " ";
    elements[i]->write(out);
  }
  out << ")";
}

/* ------------------------------------------------------------------------- */

ExpressionArrayReference::ExpressionArrayReference(Co<ExpressionArray> array_expression) :
  array_expression(array_expression) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArrayReference::evaluate(Environment &env) const {
  return Co<Expression>(new ExpressionArrayReference(array_expression));
}

/* ------------------------------------------------------------------------- */

void ExpressionArrayReference::write(ostream &out) const {
  out << "(ARRAYREF ";
  array_expression->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

Co<ExpressionArray> ExpressionArrayReference::getArray() {
  return array_expression;
}

/* ------------------------------------------------------------------------- */

Co<const ExpressionArray> ExpressionArrayReference::getArray() const {
  return array_expression;
}

/* ------------------------------------------------------------------------- */

ExpressionArrayLiteral::ExpressionArrayLiteral(const std::vector<Co<Expression> >& items) :
  items(items) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArrayLiteral::evaluate(Environment &env) const {
  Co<ExpressionArray> array = Co<Expression>(new ExpressionArray(items.size(), Co<Expression>(ExpressionUnit::getSingleton())));
  for (unsigned int i = 0; i < items.size(); ++i) {
    array->setElement(i, items[i]->evaluate(env));
  }

  return Co<Expression>(new ExpressionArrayReference(array));
}

/* ------------------------------------------------------------------------- */

void ExpressionArrayLiteral::write(ostream &out) const {
  out << "(ARRAYLITERAL";
  for (unsigned int i = 0; i < items.size(); ++i) {
    out << " ";
    items[i]->write(out);
  }
  out << ")";
}

/* ------------------------------------------------------------------------- */

ExpressionArrayConstructor::ExpressionArrayConstructor(Co<Expression> fill_expression, Co<Expression> length_expression) :
  Expression(),
  fill_expression(fill_expression),
  length_expression(length_expression) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArrayConstructor::evaluate(Environment &env) const {
  Co<Expression> length_value = length_expression->evaluate(env);
  ExpressionInteger *length = dynamic_cast<ExpressionInteger *>(length_value.GetPointer());

  if (!length) {
    throw MessagedException(length_expression->getSourceLocation().toAnchor() + ": An array expects its number of elements to be an integer. " + length_expression->getSource() + " is not an integer.");
  }

  Co<ExpressionArray> array = Co<Expression>(new ExpressionArray(length->toInteger(), Co<Expression>(ExpressionUnit::getSingleton())));
  for (int i = 0; i < length->toInteger(); ++i) {
    array->setElement(i, fill_expression->evaluate(env));
  }

  return Co<Expression>(new ExpressionArrayReference(array));
}

/* ------------------------------------------------------------------------- */

void ExpressionArrayConstructor::write(ostream &out) const {
  out << "(MAKEARRAY ";
  length_expression->write(out);
  out << " ";
  fill_expression->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

ExpressionArraySize::ExpressionArraySize() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArraySize::evaluate(Environment &env) const {
  Co<ExpressionClosure> array_closure = env["array"];
  if (array_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function size to be given a value named array. No value named array is defined.");
  }

  Co<Expression> array_value = array_closure->evaluate(env);
  ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(array_value.GetPointer());

  if (!array) {
    throw MessagedException(array_expression->getSourceLocation().toAnchor() + ": I expect function size to be applied to an array. " + array_expression->getSource() + " is not an array.");
  }

  return Co<Expression>(new ExpressionInteger(array->getArray()->getSize()));
}

/* ------------------------------------------------------------------------- */

void ExpressionArraySize::write(ostream &out) const {
  out << "(size array)";
}

/* ------------------------------------------------------------------------- */

ExpressionArraySubscript::ExpressionArraySubscript(Co<Expression> array_expression, Co<Expression> index_expression) :
  Expression(),
  array_expression(array_expression),
  index_expression(index_expression) {
}

/* ------------------------------------------------------------------------- */

Co<ExpressionArrayReference> ExpressionArraySubscript::evaluateArrayReference(Environment &env) const {
  Co<Expression> array_value = array_expression->evaluate(env);
  ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(array_value.GetPointer());
  if (!array) {
    throw MessagedException(array_expression->getSourceLocation().toAnchor() + ": Operator [] expects to be applied to an array. " + array_expression->getSource() + " is not an array.");
  }

  return Co<ExpressionArrayReference>(array_value);
}

/* ------------------------------------------------------------------------- */

Co<ExpressionInteger> ExpressionArraySubscript::evaluateIndex(Environment &env, Co<ExpressionArrayReference> array) const {
  Co<Expression> index_value = index_expression->evaluate(env);
  ExpressionInteger *index = dynamic_cast<ExpressionInteger *>(index_value.GetPointer());
  if (!index) {
    throw MessagedException(index_expression->getSourceLocation().toAnchor() + ": Operator [] expects index to be an integer. " + index_expression->getSource() + " is not an integer.");
  }

  if (index->toInteger() < 0) {
    index->setInteger(index->toInteger() + array->getArray()->getSize());
  }

  if (index->toInteger() < 0 || index->toInteger() >= array->getArray()->getSize()) {
    std::stringstream ss;
    ss << index_expression->getSourceLocation().toAnchor();
    ss << ": Operator [] expects a valid index. The array has ";
    ss << array->getArray()->getSize();
    ss << " elements, indexed 0 through " << (array->getArray()->getSize() - 1) << ". " << index_expression->getSource() << " is not a valid index.";
    throw MessagedException(ss.str());
  }

  return Co<ExpressionInteger>(index_value);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArraySubscript::evaluate(Environment &env) const {
  Co<ExpressionArrayReference> array = evaluateArrayReference(env);
  Co<ExpressionInteger> index = evaluateIndex(env, array);
  return (*array->getArray())[index->toInteger()];
}

/* ------------------------------------------------------------------------- */

void ExpressionArraySubscript::write(ostream &out) const {
  out << "(SUBSCRIPT ";
  array_expression->write(out);
  out << " ";
  index_expression->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
