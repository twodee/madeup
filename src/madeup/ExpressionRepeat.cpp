#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionRepeat.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRepeat::ExpressionRepeat(Co<Expression> niterations, Co<Expression> body) :
  Expression(),
  niterations(niterations),
  body(body) {
  isEmitter(false);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRepeat::evaluate(Environment &env) const {
  Co<Expression> r = Co<Expression>(ExpressionUnit::getSingleton());

  Co<Expression> v = niterations->evaluate(env);
  ExpressionInteger *niterations_value = dynamic_cast<ExpressionInteger *>(v.GetPointer());
  if (!niterations_value) {
    throw MessagedException(niterations->getSourceLocation().toAnchor() + ": A repeat loop expects its number of iterations to be an integer. " + niterations->getSource() + " is not an integer.");
  }

  int n = niterations_value->toInteger();
  for (int i = 0; i < n; ++i) {
    r = body->evaluate(env);
  }

  return r;
}

/* ------------------------------------------------------------------------- */

void ExpressionRepeat::write(ostream &out) const {
  out << "(repeat ";
  niterations->write(out);
  out << " ";
  body->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
