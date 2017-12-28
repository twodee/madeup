#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionArray::ExpressionArray(int nelements, Co<Expression> fill) :
  Expression() {
  for (int i = 0; i < nelements; ++i) {
    elements.push_back(fill);
  }
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArray::evaluate(Environment &env) const {
  return Co<Expression>((ExpressionArray *) (this));
}

/* ------------------------------------------------------------------------- */

int ExpressionArray::getSize() const {
  return elements.size();
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionArray::operator[](int i) {
  if (i < 0 || (unsigned int) i >= elements.size()) {
    throw MessagedException("ack!");
  }
  return elements[i];
}

/* ------------------------------------------------------------------------- */

void ExpressionArray::setElement(int i, Co<Expression> expr) {
  elements[i] = expr;
}

/* ------------------------------------------------------------------------- */

void ExpressionArray::append(td::Co<Expression> expr) {
  elements.push_back(expr); 
}

/* ------------------------------------------------------------------------- */

void ExpressionArray::write(ostream &out) const {
  out << "(ARRAY";
  for (int i = 0; (unsigned int) i < elements.size(); ++i) {
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

}
