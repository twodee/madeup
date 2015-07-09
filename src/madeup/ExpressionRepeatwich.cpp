#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionRepeatwich.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionRepeatwich::ExpressionRepeatwich(Co<Expression> nmores,
                                           Co<Expression> more,
                                           Co<Expression> less) :
  Expression(),
  nmores(nmores),
  more(more),
  less(less) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRepeatwich::evaluate(Environment &env) const {
  Co<Expression> r = Co<Expression>(ExpressionUnit::getSingleton());

  Co<Expression> v = nmores->evaluate(env);
  ExpressionInteger *nmores_value = dynamic_cast<ExpressionInteger *>(v.GetPointer());
  if (!nmores_value) {
    throw MessagedException(nmores->getSourceLocation().toAnchor() + ": A repeatwich loop expects its number of iterations to be an integer. " + nmores->getSource() + " is not an integer.");
  }

  int n = nmores_value->toInteger();
  if (n > 0) {
    r = more->evaluate(env);
    for (int i = 1; i < n; ++i) {
      less->evaluate(env);
      r = more->evaluate(env);
    }
  }

  return r;
}

/* ------------------------------------------------------------------------- */

void ExpressionRepeatwich::write(ostream &out) const {
  out << "(repeatwich ";
  nmores->write(out);
  out << " ";
  more->write(out);
  out << " ";
  less->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
