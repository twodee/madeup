#include <sstream>

#include "madeup/ExpressionArray.h"
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
  return evaluate_helper(left, right, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionEqual::evaluate_helper(Co<Expression> l,
                                                Co<Expression> r,
                                                const std::string &source,
                                                const SourceLocation &location,
                                                Environment &env) {
  Co<Expression> l_value = l->evaluate(env);
  Co<Expression> r_value = r->evaluate(env);

  // Integers
  ExpressionInteger *l_integer = dynamic_cast<ExpressionInteger *>(l_value.GetPointer());
  ExpressionInteger *r_integer = dynamic_cast<ExpressionInteger *>(r_value.GetPointer());
  if (l_integer && r_integer) {
    return Co<Expression>(new ExpressionBoolean(l_integer->toInteger() == r_integer->toInteger()));
  }

  // Reals
  ExpressionInteger *l_real = dynamic_cast<ExpressionInteger *>(l_value.GetPointer());
  ExpressionInteger *r_real = dynamic_cast<ExpressionInteger *>(r_value.GetPointer());
  if (l_real && r_real) {
    return Co<Expression>(new ExpressionBoolean(l_real->toReal() == r_real->toReal()));
  }

  // Booleans
  ExpressionBoolean *l_boolean = dynamic_cast<ExpressionBoolean *>(l_value.GetPointer());
  ExpressionBoolean *r_boolean = dynamic_cast<ExpressionBoolean *>(r_value.GetPointer());
  if (l_boolean && r_boolean) {
    return Co<Expression>(new ExpressionBoolean(l_boolean->toBoolean() == r_boolean->toBoolean()));
  }
  
  // Strings
  ExpressionString *l_string = dynamic_cast<ExpressionString *>(l_value.GetPointer());
  ExpressionString *r_string = dynamic_cast<ExpressionString *>(r_value.GetPointer());
  if (l_string && r_string) {
    return Co<Expression>(new ExpressionBoolean(l_string->getString() == r_string->getString()));
  }

  // Both are arrays
  ExpressionArrayReference *l_array = dynamic_cast<ExpressionArrayReference *>(l_value.GetPointer());
  ExpressionArrayReference *r_array = dynamic_cast<ExpressionArrayReference *>(r_value.GetPointer());
  if (l_array && r_array) {
    int nitems = l_array->getArray()->getSize();
    if (nitems == r_array->getArray()->getSize()) {
      for (int i = 0; i < nitems; ++i) {
        Co<Expression> is_same = evaluate_helper((*l_array->getArray())[i],
                                               (*r_array->getArray())[i],
                                               source, location, env);
        ExpressionBoolean *b = dynamic_cast<ExpressionBoolean *>(is_same.GetPointer());
        if (!b->toBoolean()) {
          return is_same;
        }
      }
      return Co<Expression>(new ExpressionBoolean(true));
    }
  } 

  return Co<Expression>(new ExpressionBoolean(false));
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
