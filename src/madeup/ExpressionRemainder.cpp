#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionRemainder.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRemainder::ExpressionRemainder(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRemainder::evaluate(Environment &env) const {
  return evaluate_helper(left, right, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRemainder::evaluate_helper(Co<Expression> l,
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
    if (r_integer->toInteger() != 0) {
      return Co<Expression>(new ExpressionInteger(l_integer->toInteger() % r_integer->toInteger()));
    } else {
      throw MessagedException(r->getSourceLocation().toAnchor() + ": I don't know how to get the remainder when dividing by 0.");
    }
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
      throw MessagedException(location.toAnchor() + ": Operator % doesn't know how to get remainders across arrays of different sizes.");
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

  throw MessagedException(location.toAnchor() + ": Operator % doesn't know how to get the remainder of " + l->getSource() + " divided by " + r->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionRemainder::write(ostream &out) const {
  out << "(% ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
