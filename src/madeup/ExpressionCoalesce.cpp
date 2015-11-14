#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionCoalesce.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionCoalesce::ExpressionCoalesce() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionCoalesce::evaluate(Environment &env) const {
  Co<ExpressionClosure> threshold_closure = env["threshold"];
  if (threshold_closure.IsNull()) {
    throw UnlocatedException("I expect function coalesce to given a parameter named threshold. No value named threshold was given.");
  }

  Co<Expression> threshold_value = threshold_closure->evaluate(env);

  ExpressionNumber *threshold = dynamic_cast<ExpressionNumber *>(threshold_value.GetPointer());
  if (threshold) {
    return Co<Expression>(new ExpressionInteger(env.coalesce(threshold->toReal())));
  }

  throw UnlocatedException("I expect function coalesce to be given a number. " + threshold_closure->getSource() + " is not a number.");
}

/* ------------------------------------------------------------------------- */

void ExpressionCoalesce::write(ostream &out) const {
  out << "(coalesce)";
}

/* ------------------------------------------------------------------------- */

}
