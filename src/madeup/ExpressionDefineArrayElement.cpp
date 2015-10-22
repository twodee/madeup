#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefineArrayElement.h"
#include "madeup/ExpressionInteger.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDefineArrayElement::ExpressionDefineArrayElement(Co<Expression> array, Co<Expression> index, Co<Expression> rhs) :
  Expression(),
  subscript(Co<ExpressionArraySubscript>(new ExpressionArraySubscript(array, index))),
  rhs(rhs) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineArrayElement::evaluate(Environment &env) const {
  Co<Expression> rhs_value = rhs->evaluate(env);
  rhs_value->setSource(rhs->getSource(), rhs->getSourceLocation());

  Co<ExpressionArrayReference> array = subscript->evaluateArrayReference(env); 
  Co<ExpressionInteger> index = subscript->evaluateIndex(env, array); 

  array->getArray()->setElement(index->toInteger(), rhs_value);
  
  return rhs_value;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineArrayElement::getRightHandSide() const {
  return rhs;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefineArrayElement::write(ostream &out) const {
  out << "(define-array-element ";
  subscript->write(out);
  out << " ";
  rhs->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
