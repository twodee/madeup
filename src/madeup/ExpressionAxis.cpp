#include "madeup/ExpressionAxis.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionAxis::ExpressionAxis() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionAxis::evaluate(Environment &env) const {
  Co<ExpressionClosure> x = env["x"];
  Co<ExpressionClosure> y = env["y"];
  Co<ExpressionClosure> z = env["z"];

  if (x.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function axis expects a value named x. No x was defined.");
  }

  if (y.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function axis expects a value named y. No y was defined.");
  }

  if (z.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": Function axis expects a value named z. No z was defined.");
  }

  Co<Expression> x_value = x->evaluate(env);
  Co<Expression> y_value = y->evaluate(env);
  Co<Expression> z_value = z->evaluate(env);

  double xd;
  ExpressionReal *x_double = dynamic_cast<ExpressionReal *>(x_value.GetPointer());
  if (x_double) {
    xd = x_double->toReal();
  } else {
    ExpressionInteger *x_int = dynamic_cast<ExpressionInteger *>(x_value.GetPointer());
    if (x_int) {
      xd = x_int->toInteger();
    } else {
      throw MessagedException(x->getSourceLocation().toAnchor() + ": Function axis expects its x parameter to be numeric.");
    }
  }

  double yd;
  ExpressionReal *y_double = dynamic_cast<ExpressionReal *>(y_value.GetPointer());
  if (y_double) {
    yd = y_double->toReal();
  } else {
    ExpressionInteger *y_int = dynamic_cast<ExpressionInteger *>(y_value.GetPointer());
    if (y_int) {
      yd = y_int->toInteger();
    } else {
      throw MessagedException(y->getSourceLocation().toAnchor() + ": Function axis expects its y parameter to be numeric.");
    }
  }

  double zd;
  ExpressionReal *z_double = dynamic_cast<ExpressionReal *>(z_value.GetPointer());
  if (z_double) {
    zd = z_double->toReal();
  } else {
    ExpressionInteger *z_int = dynamic_cast<ExpressionInteger *>(z_value.GetPointer());
    if (z_int) {
      zd = z_int->toInteger();
    } else {
      throw MessagedException(z->getSourceLocation().toAnchor() + ": Function axis expects its z parameter to be numeric.");
    }
  }

  env.axis(xd, yd, zd);
  return Co<Expression>(new ExpressionUnit());
}

/* ------------------------------------------------------------------------- */

void ExpressionAxis::write(ostream &out) const {
  out << "(axis x y z)";
}

/* ------------------------------------------------------------------------- */

}
