#include <iostream>

#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionWhere.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionWhere::ExpressionWhere() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionWhere::evaluate(Environment &env) const {
  QVector3<float> position = env.getTurtle().position;
  Co<ExpressionArray> new_array(new ExpressionArray(3));
  for (int i = 0; i < 3; ++i) {
    new_array->setElement(i, Co<Expression>(new ExpressionReal(position[i])));
  }
  return Co<Expression>(new ExpressionArrayReference(new_array));
}

/* ------------------------------------------------------------------------- */

void ExpressionWhere::write(ostream &out) const {
  out << "(where)";
}

/* ------------------------------------------------------------------------- */

}
