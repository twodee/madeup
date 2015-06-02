#include <sstream>

#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionEqual.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionString.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionEqual::ExpressionEqual(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionEqual::evaluate(Environment &env) const {
  Co<Expression> lvalue = left->evaluate(env);
  Co<Expression> rvalue = right->evaluate(env);

  ExpressionString *lstring = dynamic_cast<ExpressionString *>(lvalue.GetPointer());
  ExpressionString *rstring = dynamic_cast<ExpressionString *>(rvalue.GetPointer());
  ExpressionReal *ldecimal = dynamic_cast<ExpressionReal *>(lvalue.GetPointer());
  ExpressionReal *rdecimal = dynamic_cast<ExpressionReal *>(rvalue.GetPointer());
  ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
  ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
  ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
  ExpressionBoolean *rboolean = dynamic_cast<ExpressionBoolean *>(rvalue.GetPointer());
  
  Co<Expression> value;
  if (lstring && rstring) {
    value = Co<Expression>(new ExpressionBoolean(lstring->getString() == rstring->getString()));
  } else if (ldecimal && rdecimal) {
    value = Co<Expression>(new ExpressionBoolean(ldecimal->toReal() == rdecimal->toReal()));
  } else if (linteger && rinteger) {
    value = Co<Expression>(new ExpressionBoolean(linteger->toInteger() == rinteger->toInteger()));
  } else if (lboolean && rboolean) {
    value = Co<Expression>(new ExpressionBoolean(lboolean->toBoolean() == rboolean->toBoolean()));
  } else {
    value = new ExpressionBoolean(false);
  }

  return value;
}

/* ------------------------------------------------------------------------- */

void ExpressionEqual::write(ostream &out) const {
  out << "(== ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
