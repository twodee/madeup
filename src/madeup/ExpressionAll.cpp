#include "madeup/ExpressionAll.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionClosure.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionAll::ExpressionAll() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionAll::evaluate(Environment &env) const {
  Co<ExpressionClosure> array_closure = env["array"];
  if (array_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function all to be given a parameter named array.");
  }

  Co<Expression> value = array_closure->evaluate(env);

  // Array
  ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(value.GetPointer());
  if (array) {
    int nitems = array->getArray()->getSize();
    for (int i = 0; i < nitems; ++i) {
      Co<Expression> element = (*array->getArray())[i];
      Co<Expression> element_value = element->evaluate(env);
      ExpressionBoolean *element_boolean = dynamic_cast<ExpressionBoolean *>(element_value.GetPointer());
      if (element_boolean) {
        if (!element_boolean->toBoolean()) {
          return Co<Expression>(new ExpressionBoolean(false));
        }
      } else {
        throw MessagedException(array_closure->getSourceLocation().toAnchor() + ": I tried applying all to an array, but I didn't know what to do with element " + element->getSource() + ".");
      }
    }
    return Co<Expression>(new ExpressionBoolean(true));
  }

  throw MessagedException(array_closure->getSourceLocation().toAnchor() + ": I don't know how to apply function all to " + array_closure->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionAll::write(ostream &out) const {
  out << "(all x)";
}

/* ------------------------------------------------------------------------- */

}
