#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionUnit::singleton;

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionUnit::getSingleton() {
  if (singleton.IsNull()) {
    singleton = Co<Expression>(new ExpressionUnit());
  }
  return singleton;
}

/* ------------------------------------------------------------------------- */

ExpressionUnit::ExpressionUnit() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionUnit::evaluate(Environment &env) const {
  return Co<Expression>(ExpressionUnit::getSingleton());
}

/* ------------------------------------------------------------------------- */

void ExpressionUnit::write(ostream &out) const {
  out << "(unit)";
}

/* ------------------------------------------------------------------------- */

}
