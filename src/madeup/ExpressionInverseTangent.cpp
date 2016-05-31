#include <math.h>

#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInverseTangent.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "twodee/Utilities.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionInverseTangent::ExpressionInverseTangent() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionInverseTangent::evaluate(Environment &env) const {
  Co<ExpressionClosure> ratio_closure = env["ratio"];
  if (ratio_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function atan expects a value named ratio. No value named ratio is defined.");
  }

  Co<Expression> v = ratio_closure->evaluate(env);

  ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (number) {
    float value = number->toReal();
    return Co<Expression>(new ExpressionReal(atanf(value) * 180.0f / td::PI));
  }

  throw MessagedException(ratio_closure->getSourceLocation().toAnchor() + ": Function atan expects a number. " + ratio_closure->getSource() + " is not a number.");
}

/* ------------------------------------------------------------------------- */

void ExpressionInverseTangent::write(ostream &out) const {
  out << "(atan x)";
}

/* ------------------------------------------------------------------------- */

ExpressionInverseTangent2::ExpressionInverseTangent2() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionInverseTangent2::evaluate(Environment &env) const {
  // Opposite
  Co<ExpressionClosure> opposite_closure = env["opposite"];
  if (opposite_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function atan2 expects a value named opposite. No value named opposite is defined.");
  }

  Co<Expression> v = opposite_closure->evaluate(env);

  ExpressionNumber *opposite = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!opposite) {
    throw MessagedException(opposite_closure->getSourceLocation().toAnchor() + ": Function atan2 expects opposite to be a number. " + opposite_closure->getSource() + " is not a number.");
  }

  float opposite_value = opposite->toReal();

  // Adjacent
  Co<ExpressionClosure> adjacent_closure = env["adjacent"];
  if (adjacent_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function atan2 expects a value named adjacent. No value named adjacent is defined.");
  }

  v = adjacent_closure->evaluate(env);
  
  ExpressionNumber *adjacent = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  if (!adjacent) {
    throw MessagedException(adjacent_closure->getSourceLocation().toAnchor() + ": Function atan2 expects adjacent to be a number. " + adjacent_closure->getSource() + " is not a number.");
  }

  float adjacent_value = adjacent->toReal();

  return Co<Expression>(new ExpressionReal(atan2f(opposite_value, adjacent_value) * 180.0f / td::PI));
}

/* ------------------------------------------------------------------------- */

void ExpressionInverseTangent2::write(ostream &out) const {
  out << "(atan2 opposite adjacent)";
}

/* ------------------------------------------------------------------------- */

}
