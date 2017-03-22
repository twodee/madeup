#include "madeup/ExpressionCenter.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionObject.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionUtilities.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionCenter::ExpressionCenter() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionCenter::evaluate(Environment &env) const {
  Co<ExpressionObject> object = ExpressionUtilities::lookup<ExpressionObject>("object", "path or mesh", "center", env);
  return object->center();
}

/* ------------------------------------------------------------------------- */

void ExpressionCenter::write(ostream &out) const {
  out << "(center)";
}

/* ------------------------------------------------------------------------- */

}
