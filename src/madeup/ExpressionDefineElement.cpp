#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefineElement.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionSubscript.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDefineElement::ExpressionDefineElement(Co<Expression> base,
                                                 Co<Expression> index,
                                                 Co<Expression> rhs) :
  Expression(),
  subscript(Co<ExpressionSubscript>(new ExpressionSubscript(base, index))),
  rhs(rhs) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineElement::evaluate(Environment &env) const {
  Co<Expression> rhs_value = rhs->evaluate(env);
  rhs_value->setSource(rhs->getSource(), rhs->getSourceLocation());

  Co<Expression> base_value = subscript->evaluateBase(env); 

  // Try as an array.
  ExpressionArrayReference *array_value = dynamic_cast<ExpressionArrayReference *>(base_value.GetPointer());
  if (!array_value) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect []-assignment to be applied to an array. " + subscript->getBase()->getSource() + " is not an array.");
  }

  Co<ExpressionInteger> index_value = subscript->evaluateIndex(env, array_value); 
  array_value->getArray()->setElement(index_value->toInteger(), rhs_value);
  return rhs_value;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineElement::getRightHandSide() const {
  return rhs;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefineElement::write(ostream &out) const {
  out << "(define-element ";
  subscript->write(out);
  out << " ";
  rhs->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
