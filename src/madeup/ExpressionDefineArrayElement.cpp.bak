#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefineArrayElement.h"
#include "madeup/ExpressionInteger.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDefineArrayElement::ExpressionDefineArrayElement(Co<ExpressionCall> subscript, Co<Expression> rhs) :
  Expression(),
  subscript(subscript),
  rhs(rhs) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineArrayElement::evaluate(Environment &env) const {
  Co<Expression> rhs_value = rhs->evaluate(env);
  rhs_value->setSource(rhs->getSource(), rhs->getSourceLocation());

  /* Co<ExpressionInteger> index = subscript->evaluateIndex(env, array);  */

  Co<ExpressionClosure> closure = env[subscript->getName()];
  Co<Expression> result = closure->evaluate(env);
  ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(result.GetPointer());
  if (array) {
    // assert only 1 param 
    if (subscript->getArity() != 1) {
      throw MessagedException(getSourceLocation().toAnchor() + ": " + "Bad subscript.");
    }

    // evaluate param
    Co<Expression> index_value = (*subscript)[0]->evaluate(env);
    ExpressionInteger *index = dynamic_cast<ExpressionInteger *>(index_value.GetPointer());
    if (!index) {
      throw MessagedException(getSourceLocation().toAnchor() + ": " + "Non-integer subscript.");
    }
    array->getArray()->setElement(index->toInteger(), rhs_value);
  
    return rhs_value;
  } else {
    throw MessagedException(getSourceLocation().toAnchor() + ": " + "Bad array");
  }
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
