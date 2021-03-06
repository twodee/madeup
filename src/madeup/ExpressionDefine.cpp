#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionUnit.h"

using std::set;
using std::string;
using std::vector;
using namespace td;

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

bool FormalParameter::isSplattable() const {
  return splats.size() > 0;
}

/* ------------------------------------------------------------------------- */

void FormalParameter::splatTo(const std::vector<std::string> &splats) {
  this->splats = splats; 
}

/* ------------------------------------------------------------------------- */

const std::vector<std::string> &FormalParameter::getSplats() const {
  return splats; 
}

/* ------------------------------------------------------------------------- */

void FormalParameter::setDefaultValue(Co<Expression> default_value) {
  this->default_value = default_value; 
}

/* ------------------------------------------------------------------------- */

Co<Expression> FormalParameter::getDefaultValue() const {
  return default_value; 
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

void ExpressionDefine::addFormal(const FormalParameter &formal) {
  formals.push_back(formal);
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

void FormalParameter::write(ostream &out) const {
  out << "(" << name;
  if (!getDefaultValue().IsNull()) {
    out << " ";
    default_value->write(out);
  }
  out << ")";
}

/* ------------------------------------------------------------------------- */

void ExpressionDefine::write(ostream &out) const {
  out << "(define " << name << " ";
  out << "(";
  for (vector<FormalParameter>::const_iterator i = formals.begin(); i != formals.end(); ++i) {
    if (i != formals.begin()) {
      out << " ";
    }
    i->write(out);
  }
  out << ") ";
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

void ExpressionDefine::splat(const std::string &fromID, const std::vector<std::string> &toIDs) {
  for (std::vector<FormalParameter>::iterator formal = formals.begin(); formal != formals.end(); ++formal) {
    if (formal->getName() == fromID) {
      formal->splatTo(toIDs);
      break;
    } 
  }
}

/* ------------------------------------------------------------------------- */

}
