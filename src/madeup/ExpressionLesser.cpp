#include <sstream>

#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionLesser.h"
#include "madeup/ExpressionReal.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionLesser::ExpressionLesser(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionLesser::evaluate(Environment &env) const {
  return evaluate_helper(left, right, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionLesser::evaluate_helper(Co<Expression> l,
                                                 Co<Expression> r,
                                                 const std::string &source,
                                                 const SourceLocation &location,
                                                 Environment &env) {
  Co<Expression> l_value = l->evaluate(env);
  Co<Expression> r_value = r->evaluate(env);

  // Integers
  ExpressionInteger *l_integer = dynamic_cast<ExpressionInteger *>(l_value.GetPointer());
  ExpressionInteger *r_integer = dynamic_cast<ExpressionInteger *>(r_value.GetPointer());
  if (l_integer && r_integer) {
    return Co<Expression>(new ExpressionBoolean(l_integer->toInteger() < r_integer->toInteger()));
  }

  // Any mix of numbers
  ExpressionNumber *l_number = dynamic_cast<ExpressionNumber *>(l_value.GetPointer());
  ExpressionNumber *r_number = dynamic_cast<ExpressionNumber *>(r_value.GetPointer());
  if (l_number && r_number) {
    return Co<Expression>(new ExpressionBoolean(l_number->toReal() < r_number->toReal()));
  }

  // Both are arrays
  ExpressionArrayReference *l_array = dynamic_cast<ExpressionArrayReference *>(l_value.GetPointer());
  ExpressionArrayReference *r_array = dynamic_cast<ExpressionArrayReference *>(r_value.GetPointer());
  if (l_array && r_array) {
    int nitems = l_array->getArray()->getSize();
    if (nitems == r_array->getArray()->getSize()) {
      Co<ExpressionArray> array(new ExpressionArray(nitems));
      for (int i = 0; i < nitems; ++i) {
        array->setElement(i, evaluate_helper((*l_array->getArray())[i], (*r_array->getArray())[i], source, location, env));
      }
      return Co<Expression>(new ExpressionArrayReference(array));
    } else {
      throw MessagedException(location.toAnchor() + ": Operator < doesn't know how to compare arrays of different sizes.");
    }
  } 

  // Left only is an array
  if (l_array) {
    int nitems = l_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, evaluate_helper((*l_array->getArray())[i], r_value, source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  // Right only is an array
  if (r_array) {
    int nitems = r_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, evaluate_helper(l_value, (*r_array->getArray())[i], source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  throw MessagedException(location.toAnchor() + ": Operator < doesn't know how to compare " + l->getSource() + " and " + r->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionLesser::write(ostream &out) const {
  out << "(< ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
