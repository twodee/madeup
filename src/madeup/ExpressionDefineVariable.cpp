#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionDefineVariable.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using std::string;
using std::vector;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDefineVariable::ExpressionDefineVariable(const string &name, Co<Expression> rhs) :
  Expression(),
  name(name),
  rhs(rhs) {
  isEmitter(false);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefineVariable::evaluate(Environment &env) const {
  Co<Expression> rhs_value = rhs->evaluate(env);
  rhs_value->setSource(rhs->getSource(), rhs->getSourceLocation());
  
  // If right-hand side is a closure already, as we would get from partial
  // function evaluation, we can just add it in directly.
  if (dynamic_cast<ExpressionClosure *>(rhs_value.GetPointer())) {
    env.add(name, rhs_value);
  }

  // Otherwise, we wrap up a binding between the ID and a closure. Why do 
  // I do this again? TODO.
  else {
    Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, rhs_value));
    define->setSource(getSource(), getSourceLocation());
    Co<ExpressionClosure> closure(new ExpressionClosure(define, env));
    env.add(name, closure);
  }

  return rhs_value;
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
  Co<Expression> rhs_value = rhs->evaluate(env);
  rhs_value->setSource(rhs->getSource(), rhs->getSourceLocation());

  /* return ExpressionUnit::getSingleton(); */
  /* Co<Expression> expr = ExpressionDefineVariable::evaluate(env); */
  /* std::cout << "expr: " << expr << std::endl; */
  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(rhs_value.GetPointer());

  if (!number) {
    throw MessagedException("Expected number!");
  }

  Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, rhs_value));
  define->setSource(getSource(), getSourceLocation());
  Co<ExpressionClosure> closure(new ExpressionClosure(define, env));
  env.add(name, closure);

  int seed = number->toInteger();
  srand(seed);
  rand();

  /* return ExpressionUnit::getSingleton(); */
  return rhs_value;
}

/* ------------------------------------------------------------------------- */

/* ExpressionDefineVariableRadius::ExpressionDefineVariableRadius(const std::string &name, Co<Expression> rhs) : */
  /* ExpressionDefineVariable(".outerRadius", rhs) { */
/* } */

/* ------------------------------------------------------------------------- */

}
