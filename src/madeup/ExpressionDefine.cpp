#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"

using std::string;
using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const FormalParameter &parameter) {
  out << parameter.getName();
  return out;
}

/* ------------------------------------------------------------------------- */

ExpressionDefine::ExpressionDefine(const string& name, Co<Expression> body) :
  Expression(),
  name(name),
  body(body),
  formals(),
  is_dynamically_scoped(false) {
}

/* ------------------------------------------------------------------------- */

ExpressionDefine::ExpressionDefine(const string& name, Co<Expression> body, const vector<FormalParameter>& formals) :
  Expression(),
  name(name),
  body(body),
  formals(formals) {
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::addFormal(const string& name, FormalParameter::evaluation_mode_t evaluation_mode) {
  formals.push_back(FormalParameter(name, evaluation_mode));
}

/* ------------------------------------------------------------------------- */

const FormalParameter& ExpressionDefine::getFormal(int i) {
  return formals[i];
}

/* ------------------------------------------------------------------------- */

unsigned int ExpressionDefine::getArity() const {
  return formals.size();
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefine::evaluate(Environment& env) const {
  Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, body, formals));
  Co<ExpressionClosure> closure(new ExpressionClosure(define, env));
  if (!env.isBound(name)) {
    env.add(name, closure);
    closure->setEnvironment(env);
  } else {
    env.add(name, closure);
  }
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefine::getBody() const {
  return body;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::write(ostream& out) const {
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

}
