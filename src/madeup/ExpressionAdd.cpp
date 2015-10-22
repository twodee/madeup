#include <sstream>

#include "madeup/ExpressionAdd.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionPrimitive.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionAdd::ExpressionAdd(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionAdd::evaluate(Environment &env) const {
  return evaluate_helper(left, right, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionAdd::evaluate_helper(Co<Expression> l,
                                              Co<Expression> r,
                                              const std::string &source,
                                              const SourceLocation &location,
                                              Environment &env) {
  Co<Expression> l_value = l->evaluate(env);
  Co<Expression> r_value = r->evaluate(env);

  // Left is a string.
  ExpressionString *l_string = dynamic_cast<ExpressionString *>(l_value.GetPointer());
  if (l_string) {
    ExpressionPrimitive *r_primitive = dynamic_cast<ExpressionPrimitive *>(r_value.GetPointer());
    if (r_primitive) {
      return Co<Expression>(new ExpressionString(l_string->getString() + r_primitive->toString()));
    } else {
      throw MessagedException(r->getSourceLocation().toAnchor() + ": Operator + doesn't know how to join a string to " + r->getSource());
    }
  }

  // Right is a string.
  ExpressionString *r_string = dynamic_cast<ExpressionString *>(r_value.GetPointer());
  if (r_string) {
    ExpressionPrimitive *l_primitive = dynamic_cast<ExpressionPrimitive *>(l_value.GetPointer());
    if (l_primitive) {
      return Co<Expression>(new ExpressionString(l_primitive->toString() + r_string->getString()));
    } else {
      throw MessagedException(l->getSourceLocation().toAnchor() + ": Operator + doesn't know how to append a string to " + l->getSource());
    }
  }

  // Integers
  ExpressionInteger *l_integer = dynamic_cast<ExpressionInteger *>(l_value.GetPointer());
  ExpressionInteger *r_integer = dynamic_cast<ExpressionInteger *>(r_value.GetPointer());
  if (l_integer && r_integer) {
    return Co<Expression>(new ExpressionInteger(l_integer->toInteger() + r_integer->toInteger()));
  }

  // Any mix of numbers
  ExpressionNumber *l_number = dynamic_cast<ExpressionNumber *>(l_value.GetPointer());
  ExpressionNumber *r_number = dynamic_cast<ExpressionNumber *>(r_value.GetPointer());
  if (l_number && r_number) {
    return Co<Expression>(new ExpressionReal(l_number->toReal() + r_number->toReal()));
  }

  // Arrays
  ExpressionArrayReference *l_array = dynamic_cast<ExpressionArrayReference *>(l_value.GetPointer());
  ExpressionArrayReference *r_array = dynamic_cast<ExpressionArrayReference *>(r_value.GetPointer());
  if (l_array && r_array) {
    int nitems = l_array->getArray()->getSize();
    if (nitems == r_array->getArray()->getSize()) {
      Co<ExpressionArray> array(new ExpressionArray(nitems, ExpressionUnit::getSingleton()));
      for (int i = 0; i < nitems; ++i) {
        array->setElement(i, evaluate_helper((*l_array->getArray())[i], (*r_array->getArray())[i], source, location, env));
      }
      return Co<Expression>(new ExpressionArrayReference(array));
    } else {
      throw MessagedException(location.toAnchor() + ": Operator + doesn't know how to add arrays of different sizes.");
    }
  } 

  // Left only is an array
  if (l_array) {
    int nitems = l_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems, ExpressionUnit::getSingleton()));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, evaluate_helper((*l_array->getArray())[i], r_value, source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  // Right only is an array
  if (r_array) {
    int nitems = r_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems, ExpressionUnit::getSingleton()));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, evaluate_helper(l_value, (*r_array->getArray())[i], source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  throw MessagedException(location.toAnchor() + ": Operator + doesn't know how to join " + l->getSource() + " and " + r->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionAdd::write(ostream &out) const {
  out << "(+ ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
