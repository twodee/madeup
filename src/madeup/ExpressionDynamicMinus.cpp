#include <sstream>

#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionCall.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDynamicMinus.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNegation.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionSubtract.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDynamicMinus::ExpressionDynamicMinus(const std::string &id, Co<Expression> right) :
  Expression(),
  id(id),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDynamicMinus::evaluate(Environment &env) const {
  Co<ExpressionClosure> closure = env[id];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I couldn't find a function or variable named " + id + ".");
  }

  Co<ExpressionDefine> define = closure->getDefine();

  // If the name takes no parameters, we've got a subtraction. Otherwise a call/negation.
  if (define->getArity() == 0) {
    Co<Expression> subtraction(new ExpressionSubtract(Co<Expression>(new ExpressionCall(id)), right));
    return subtraction->evaluate(env);
  } else {
    Co<ExpressionCall> call(new ExpressionCall(id));
    call->addParameter(Co<Expression>(new ExpressionNegation(right)));
    return call->evaluate(env);
  }

  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionDynamicMinus::write(ostream &out) const {
  out << "(-? " << id;
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
