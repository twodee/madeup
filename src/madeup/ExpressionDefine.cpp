#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"

using std::string;
using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const FormalParameter &parameter) {
  out << parameter.GetName();
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

void ExpressionDefine::AddFormal(const string& name, FormalParameter::evaluation_mode_t evaluation_mode) {
  formals.push_back(FormalParameter(name, evaluation_mode));
}

/* ------------------------------------------------------------------------- */

const FormalParameter& ExpressionDefine::GetFormal(int i) {
  return formals[i];
}

/* ------------------------------------------------------------------------- */

unsigned int ExpressionDefine::GetArity() const {
  return formals.size();
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefine::Evaluate(Environment& env) {
  Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, body, formals));
  Co<ExpressionClosure> closure(new ExpressionClosure(define, env));
  if (!env.IsBound(name)) {
    env.Add(name, closure);
    closure->SetEnvironment(env);
  } else {
    env.Add(name, closure);
  }
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDefine::GetBody() const {
  return body;
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::Write(ostream& out) const {
  out << "(define " << name << " ";
  for (vector<FormalParameter>::const_iterator i = formals.begin(); i != formals.end(); ++i) {
    out << *i << " ";
  }
  body->Write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::IsDynamicallyScoped(bool enable) {
  is_dynamically_scoped = enable;
}

/* ------------------------------------------------------------------------- */

bool ExpressionDefine::IsDynamicallyScoped() const {
  return is_dynamically_scoped;
}

/* ------------------------------------------------------------------------- */

}
