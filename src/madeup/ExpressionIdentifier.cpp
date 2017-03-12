#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionIdentifier.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionUnknown.h"

using std::string;
using std::vector;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionIdentifier::ExpressionIdentifier(const string &name) :
  Expression(),
  name(name) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionIdentifier::evaluate(Environment &env) const {
  env.checkTimeout(getSourceLocation());

  Co<ExpressionClosure> closure = env[name];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I couldn't find a function or variable named " + name + ".");
  }

  return closure;
}

/* ------------------------------------------------------------------------- */

void ExpressionIdentifier::write(ostream &out) const {
  out << "(identifier " << name << ")";
}

/* ------------------------------------------------------------------------- */

const std::string &ExpressionIdentifier::getName() const {
  return name;
}

/* ------------------------------------------------------------------------- */

}
