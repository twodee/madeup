#include "madeup/ExpressionAppend.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionUtilities.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionAppend::ExpressionAppend() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionAppend::evaluate(Environment &env) const {
  Co<ExpressionArrayReference> array_ref = ExpressionUtilities::lookup<ExpressionArrayReference>("array", "array", "append", env);
  Co<Expression> item = ExpressionUtilities::lookup<Expression>("item", "expression", "append", env);
  array_ref->getArray()->append(item);
  return ExpressionUnit::getSingleton();
}

/* ------------------------------------------------------------------------- */

void ExpressionAppend::write(ostream &out) const {
  out << "(append array)";
}

/* ------------------------------------------------------------------------- */

}
