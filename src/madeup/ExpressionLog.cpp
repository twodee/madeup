#include <math.h>

#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionLog.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionLog::ExpressionLog() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionLog::evaluate(Environment &env) const {
  Co<ExpressionClosure> base_closure = env["base"];
  Co<ExpressionClosure> x_closure = env["x"];
  if (base_closure.IsNull() || x_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect log to be given two parameters: a base and a value x whose logarithm you want to compute.");
  }

  return evaluate_helper(base_closure, x_closure, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionLog::evaluate_helper(Co<Expression> base,
                                              Co<Expression> x,
                                              const std::string &source,
                                              const SourceLocation &location,
                                              Environment &env) {
  Co<Expression> base_value = base->evaluate(env);
  Co<Expression> x_value = x->evaluate(env);

  // Any mix of numbers
  ExpressionNumber *base_number = dynamic_cast<ExpressionNumber *>(base_value.GetPointer());
  ExpressionNumber *x_number = dynamic_cast<ExpressionNumber *>(x_value.GetPointer());
  if (base_number && x_number) {
    if (log(base_number->toReal()) != 0.0f) {
      return Co<Expression>(new ExpressionReal(log(x_number->toReal()) / log(base_number->toReal())));
    } else {
      throw MessagedException(base->getSourceLocation().toAnchor() + ": I don't know how to take a log of base " + base->getSource() + ".");
    }
  }

  // Both are arrays
  ExpressionArrayReference *base_array = dynamic_cast<ExpressionArrayReference *>(base_value.GetPointer());
  ExpressionArrayReference *x_array = dynamic_cast<ExpressionArrayReference *>(x_value.GetPointer());
  if (base_array && x_array) {
    int nitems = base_array->getArray()->getSize();
    if (nitems == x_array->getArray()->getSize()) {
      Co<ExpressionArray> array(new ExpressionArray(nitems));
      for (int i = 0; i < nitems; ++i) {
        array->setElement(i, evaluate_helper((*base_array->getArray())[i], (*x_array->getArray())[i], source, location, env));
      }
      return Co<Expression>(new ExpressionArrayReference(array));
    } else {
      throw MessagedException(location.toAnchor() + ": I don't know how to compute the log of arrays of different sizes.");
    }
  } 

  // Left only is an array
  if (base_array && x_number) {
    int nitems = base_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems, ExpressionUnit::getSingleton()));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, evaluate_helper((*base_array->getArray())[i], x_value, source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  // Right only is an array
  if (base_number && x_array) {
    int nitems = x_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems, ExpressionUnit::getSingleton()));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, evaluate_helper(base_value, (*x_array->getArray())[i], source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  throw MessagedException(location.toAnchor() + ": I don't know to compute the log of " + x->getSource() + " base " + base->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionLog::write(ostream &out) const {
  out << "(log x)";
}

/* ------------------------------------------------------------------------- */

}
