#ifndef EXPRESSIONUTILITIES_H
#define EXPRESSIONUTILITIES_H

#include "madeup/Environment.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionUtilities {
  public:
    template<typename T>
    static td::Co<T> lookup(const std::string &id,
                            const std::string &type_descriptor,
                            const std::string &referer,
                            Environment &env) {

      // Grab the closure bound to this identifier.
      td::Co<ExpressionClosure> closure = env[id];
      if (closure.IsNull()) {
        throw UnlocatedException("I expect function " + referer + " to be given a parameter named " + id + ". No value named " + id + " was given.");
      }

      // Evaluate the closure under the given environment.
      td::Co<Expression> value = closure->evaluate(env);

      // Get in the form of the desired type.
      T *typed_value = dynamic_cast<T *>(value.GetPointer());
      if (!typed_value) {
        throw UnlocatedException("I expect function " + referer + " to be given a " + type_descriptor + ". " + closure->getSource() + " is not a " + type_descriptor + ".");
      }

      return value;
    }
};

/* ------------------------------------------------------------------------- */

}
#endif
