#include <sstream>

#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionNot.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionNot::ExpressionNot(Co<Expression> left) :
  Expression(),
  left(left) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNot::evaluate(Environment &env) const {
  return evaluate_helper(left, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNot::evaluate_helper(Co<Expression> e,
                                              const std::string &source,
                                              const SourceLocation &location,
                                              Environment &env) {
  Co<Expression> value = e->evaluate(env);

  // Boolean
  ExpressionBoolean *e_boolean = dynamic_cast<ExpressionBoolean *>(value.GetPointer());
  if (e_boolean) {
    return Co<Expression>(new ExpressionBoolean(!e_boolean->toBoolean()));
  }

  // Array
  ExpressionArrayReference *e_array = dynamic_cast<ExpressionArrayReference *>(value.GetPointer());
  if (e_array) {
    int nitems = e_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, evaluate_helper((*e_array->getArray())[i], source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  throw MessagedException(e->getSourceLocation().toAnchor() + ": I don't know how to apply operator not to " + e->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionNot::write(ostream &out) const {
  out << "(not ";
  left->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
