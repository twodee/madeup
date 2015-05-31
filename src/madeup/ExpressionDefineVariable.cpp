#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefineVariable.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionNumber.h"

using std::string;
using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDefineVariable::ExpressionDefineVariable(const string& name, Co<Expression> rhs) :
  Expression(),
  name(name),
  rhs(rhs) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineVariable::Evaluate(Environment& env) {
  Co<Expression> rhs_value = rhs->Evaluate(env);
  rhs_value->SetSource(rhs->GetSource(), rhs->GetSourceLocation());
  Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, rhs_value));
  define->SetSource(GetSource(), GetSourceLocation());
  Co<ExpressionClosure> closure(new ExpressionClosure(define, env));
  env.Add(name, closure);
  return rhs_value;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineVariable::GetRightHandSide() const {
  return rhs;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefineVariable::Write(ostream& out) const {
  out << "(define-variable " << name << " ";
  rhs->Write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

ExpressionDefineVariableSeed::ExpressionDefineVariableSeed(const std::string& name, Co<Expression> rhs) :
  ExpressionDefineVariable(name, rhs) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineVariableSeed::Evaluate(Environment& env) {
  Co<Expression> expr = ExpressionDefineVariable::Evaluate(env);
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(expr.GetPointer());

  if (!number) {
    throw MessagedException("Expected number!");
  }

  int seed = number->GetInteger();
  srand(seed);

  return expr;
}

/* ------------------------------------------------------------------------- */

}
