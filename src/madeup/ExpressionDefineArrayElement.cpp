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

Co<Expression> ExpressionDefineArrayElement::Evaluate(Environment& env) {
  Co<Expression> rhs_value = rhs->Evaluate(env);
  rhs_value->SetSource(rhs->GetSource(), rhs->GetStartLine(), rhs->GetStartIndex(), rhs->GetEndLine(), rhs->GetEndIndex());

  Co<ExpressionArrayReference> array = subscript->EvaluateArrayReference(env); 
  Co<ExpressionInteger> index = subscript->EvaluateIndex(env, array); 

  array->GetArray()->SetElement(index->GetInteger(), rhs_value);
  
  return rhs_value;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineArrayElement::GetRightHandSide() const {
  return rhs;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefineArrayElement::Write(ostream& out) const {
  out << "(define-array-element ";
  subscript->Write(out);
  out << " ";
  rhs->Write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
