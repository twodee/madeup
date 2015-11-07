#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionUnit.h"

using std::set;
using std::string;
using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

FormalParameter::FormalParameter(const std::string &name,
                evaluation_mode_t evaluation_mode) :
  name(name),
  evaluation_mode(evaluation_mode) {
}

/* ------------------------------------------------------------------------- */

FormalParameter::evaluation_mode_t FormalParameter::getEvaluationMode() const {
  return evaluation_mode;
}

/* ------------------------------------------------------------------------- */

const std::string &FormalParameter::getName() const {
  return name;
}

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const FormalParameter &parameter) {
  out << parameter.getName();
  return out;
}

/* ------------------------------------------------------------------------- */

ExpressionDefine::ExpressionDefine(const string &name, Co<Expression> body) :
  Expression(),
  name(name),
  body(body),
  formals(),
  is_dynamically_scoped(false) {
}

/* ------------------------------------------------------------------------- */

ExpressionDefine::ExpressionDefine(const string &name, Co<Expression> body, const vector<FormalParameter> &formals) :
  Expression(),
  name(name),
  body(body),
  formals(formals) {
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::addFormal(const string &name, FormalParameter::evaluation_mode_t evaluation_mode) {
  formals.push_back(FormalParameter(name, evaluation_mode));
}

/* ------------------------------------------------------------------------- */

const FormalParameter &ExpressionDefine::getFormal(int i) {
  return formals[i];
}

/* ------------------------------------------------------------------------- */

unsigned int ExpressionDefine::getArity() const {
  return formals.size();
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefine::evaluate(Environment &env) const {
  Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, body, formals));
  Co<ExpressionClosure> closure(new ExpressionClosure(define, env));

  // For a function to be recursive and self-aware, it needs to be available in
  // its own environment. We update the environment first and then give the
  // expanded environment to the function's closure.
  if (!env.isBound(name)) {
    env.add(name, closure);
    closure->setEnvironment(env);
  }

  // If this function had a previous definition, it will already know about
  // itself. We just update the entry.
  else {
    env.add(name, closure);
  }

  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefine::getBody() const {
  return body;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::write(ostream &out) const {
  out << "(define " << name << " ";
  for (vector<FormalParameter>::const_iterator i = formals.begin(); i != formals.end(); ++i) {
    out << *i << " ";
  }
  body->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::isDynamicallyScoped(bool enable) {
  is_dynamically_scoped = enable;
}

/* ------------------------------------------------------------------------- */

bool ExpressionDefine::isDynamicallyScoped() const {
  return is_dynamically_scoped;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::predeclare(Environment &env) const {
  if (!env.isBound(name)) {
    Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, ExpressionUnit::getSingleton()));
    Co<ExpressionClosure> closure(new ExpressionClosure(define));
    env.add(name, closure);
  }
}

/* ------------------------------------------------------------------------- */

set<string> ExpressionDefine::getFormalNames() const {
  set<string> identifiers;

  for (vector<FormalParameter>::const_iterator i = formals.begin();
       i != formals.end();
       ++i) {
    identifiers.insert(i->getName());
  }

  return identifiers;
}

/* ------------------------------------------------------------------------- */

}
