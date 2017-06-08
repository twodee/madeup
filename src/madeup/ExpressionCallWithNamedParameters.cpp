#include "madeup/ExpressionCallWithNamedParameters.h"
#include "madeup/ExpressionClosure.h"

using std::pair;
using std::set;
using std::string;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionCallWithNamedParameters::ExpressionCallWithNamedParameters(const string &name) :
  Expression(),
  name(name),
  bindings() {
}

/* ------------------------------------------------------------------------- */

void ExpressionCallWithNamedParameters::addParameter(const std::string &name, Co<Expression> parameter) {
  bindings.insert(binding_t(name, parameter));
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionCallWithNamedParameters::evaluate(Environment &env) const {
  env.checkTimeout(getSourceLocation());

  Co<ExpressionClosure> closure = env[name];
  /* std::cout << "getSource(): " << getSource() << std::endl; */
  /* std::cout << "getSourceLocation(): " << getSourceLocation().toAnchor() << std::endl; */
  /* std::cout << "closure->getSourceLocation(): " << closure->getSourceLocation().toAnchor() << std::endl; */
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": No function named " + name + " is defined.");
  }
  /* closure->setSource(getSource(), getSourceLocation()); */

  Co<ExpressionDefine> define = closure->getDefine();
  Co<Environment> closure_env(new Environment(*closure->getEnvironment()));

  // Before trying to execute this business, let's make certain the user didn't
  // pass any named parameters that don't apply. Having extras isn't going to
  // stop us from executing the function, but it probably means the caller has
  // some different intent.
  set<string> formal_names = define->getFormalNames();
  for (bindings_t::const_iterator i = bindings.begin();
       i != bindings.end();
       ++i) {
    if (formal_names.find(i->first) == formal_names.end()) {
      throw MessagedException(getSourceLocation().toAnchor() + ": I saw that you passed a parameter named " + i->first + " to function " + define->getName() + ", but " + define->getName() + " doesn't accept a parameter named " + i->first + ".");
    }
  }

  // Having cleared the appropriateness of the bindings, let's start evaluating.
  for (unsigned int i = 0; i < define->getArity(); ++i) {
    const FormalParameter &formal = define->getFormal(i);
    bool is_lazy = formal.getEvaluationMode() == FormalParameter::LAZY;

    // Look up parameter name in enumerated bindings first. If we find an
    // explicit parameter with that name, use its value.
    bindings_t::const_iterator match = bindings.find(formal.getName());
    Co<ExpressionDefine> parameter_define;
    Co<ExpressionDefine> parameter_closure;
    if (match != bindings.end()) {
      parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), is_lazy ? match->second : match->second->evaluate(env)));
      parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment()));
      parameter_closure->setSource(match->second->getSource(), match->second->getSourceLocation());
    } else if (!formal.getDefaultValue().IsNull()) {
      parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), formal.getDefaultValue()->evaluate(env)));
      parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, Environment()));
      parameter_closure->setSource(formal.getDefaultValue()->getSource(), formal.getDefaultValue()->getSourceLocation());

    // UPDATE: disallow implicits. They work, but their benefit is small and
    // risk of confusion is too high.
    // If no such parameter was enumerated, checking for a binding with that
    // name in the surrounding environment at the time of the call.
    /* else if (env.isBound(formal.getName())) { */
      /* Co<Expression> closure = env[formal.getName()]; */
      /* parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), is_lazy ? closure : closure->evaluate(env))); */
      /* parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment())); */
      /* parameter_closure->setSource(closure->getSource(), closure->getSourceLocation()); */
    } else {
      throw MessagedException(getSourceLocation().toAnchor() + ": Function " + define->getName() + " expects a parameter named " + formal.getName() + ". No such parameter was provided and no variable with that name was defined.");
    }

    closure_env->replace(define->getFormal(i).getName(), parameter_closure);
  }

  try {
    return closure->evaluate(*closure_env);
  } catch (Co<Expression> return_value) {
    return return_value;
  } catch (UnlocatedException e) {
    throw MessagedException(getSourceLocation().toAnchor() + ": " + e.GetMessage());
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionCallWithNamedParameters::assign(Environment &env, Co<Expression> value) const {
  if (bindings.size() > 0) {
    Expression::assign(env, value);
  } else {
    Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(name, value));
    env.add(name, Co<ExpressionClosure>(new ExpressionClosure(define, env)));
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionCallWithNamedParameters::write(ostream &out) const {
  out << "(call-with-names " << name;
  for (bindings_t::const_iterator i = bindings.begin(); i != bindings.end(); ++i) {
    out << " " << i->first << " " << *i->second;
  }
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
