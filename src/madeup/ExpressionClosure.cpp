#include "madeup/ExpressionClosure.h"

using std::string;
using namespace td;

namespace madeup {

/* ----------------------------------------------------------------------- */

ExpressionClosure::ExpressionClosure(Co<ExpressionDefine> define) :
  define(define),
  env(Co<Environment>(new Environment())) { 
}

/* ------------------------------------------------------------------------- */

ExpressionClosure::ExpressionClosure(Co<ExpressionDefine> define, Co<Environment> shared_env) :
  Expression(),
  define(define),
  env(shared_env) {
}

/* ------------------------------------------------------------------------- */

ExpressionClosure::ExpressionClosure(Co<ExpressionDefine> define, const Environment &env) :
  Expression(),
  define(define),
  env(new Environment(env)) {
}

/* ------------------------------------------------------------------------- */

void ExpressionClosure::setEnvironment(const Environment &env) {
  this->env = Co<Environment>(new Environment(env));
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionClosure::evaluate(Environment &env) const {
  // Assumes that the environment being passed is a modification of its own,
  // including the state at the time it was created and any temporaries for
  // parameters.
 
  return define->getBody()->evaluate(env);
}

/* ------------------------------------------------------------------------- */

Co<Environment> ExpressionClosure::getEnvironment() {
  return env;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionDefine> ExpressionClosure::getDefine() {
  return define;
}

/* ------------------------------------------------------------------------- */

void ExpressionClosure::setDefine(Co<ExpressionDefine> define) {
  this->define = define;
}

/* ------------------------------------------------------------------------- */

void ExpressionClosure::write(ostream &out) const {
  out << "(closure ";
  define->getBody()->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
