#include "madeup/ExpressionBox.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionBox::ExpressionBox() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBox::evaluate(Environment &env) const {
  env.box();
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionBox::write(ostream &out) const {
  out << "(box)";
}

/* ------------------------------------------------------------------------- */

}
