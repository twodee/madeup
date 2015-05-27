#include "madeup/ExpressionClosure.h"

using std::string;

namespace madeup {

/* ----------------------------------------------------------------------- */

ExpressionClosure::ExpressionClosure(Co<ExpressionDefine> define, const Environment& env) :
  Expression(),
  define(define) {
  this->env = Co<Environment>(new Environment(env));
}

/* ------------------------------------------------------------------------- */

void ExpressionClosure::SetEnvironment(const Environment& env) {
  this->env = Co<Environment>(new Environment(env));
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionClosure::Evaluate(Environment& env) {
  // Assumes that the environment being passed is a modification of its own,
  // including the state at the time it was created and any temporaries for
  // parameters.
 
  /* std::cout << "------------------------------------------------" << std::endl; */
  /* std::cout << "in closure" << std::endl; */
  /* std::cout << "define->GetName(): " << define->GetName() << std::endl; */
  /* std::cout << "define->GetBody(): " << define->GetBody() << std::endl; */
  /* std::cout << env << std::endl; */
  /* std::cout << "about to print env" << std::endl; */
  /* std::cout << *this->env << std::endl; */
  /* std::cout << "printed env" << std::endl; */
  return define->GetBody()->Evaluate(env);
}

/* ------------------------------------------------------------------------- */

Co<Environment> ExpressionClosure::GetEnvironment() {
  return env;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionDefine> ExpressionClosure::GetDefine() {
  return define;
}

/* ------------------------------------------------------------------------- */

void ExpressionClosure::SetDefine(Co<ExpressionDefine> define) {
  this->define = define;
}

/* ------------------------------------------------------------------------- */

void ExpressionClosure::Write(ostream& out) const {
  out << "(closure ";
  define->GetBody()->Write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
