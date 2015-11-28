#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionDefineVariable.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using std::string;
using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDefineVariable::ExpressionDefineVariable(const string &name, Co<Expression> rhs) :
  Expression(),
  name(name),
  rhs(rhs) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineVariable::evaluate(Environment &env) const {
  Co<Expression> rhs_value = rhs->evaluate(env);
  rhs_value->setSource(rhs->getSource(), rhs->getSourceLocation());
  Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, rhs_value));
  define->setSource(getSource(), getSourceLocation());
  Co<ExpressionClosure> closure(new ExpressionClosure(define, env));
  env.add(name, closure);
  return ExpressionUnit::getSingleton();
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineVariable::getRightHandSide() const {
  return rhs;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefineVariable::write(ostream &out) const {
  out << "(define-variable " << name << " ";
  rhs->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

ExpressionDefineVariableSeed::ExpressionDefineVariableSeed(const std::string &name, Co<Expression> rhs) :
  ExpressionDefineVariable(name, rhs) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineVariableSeed::evaluate(Environment &env) const {
  Co<Expression> expr = ExpressionDefineVariable::evaluate(env);
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(expr.GetPointer());

  if (!number) {
    throw MessagedException("Expected number!");
  }

  int seed = number->toInteger();
  srand(seed);
  rand();

  return expr;
}

/* ------------------------------------------------------------------------- */

ExpressionDefineVariableRadius::ExpressionDefineVariableRadius(const std::string &name, Co<Expression> rhs) :
  ExpressionDefineVariable(".outerRadius", rhs) {
}

/* ------------------------------------------------------------------------- */

}
