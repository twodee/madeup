#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionCall.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"

using std::string;
using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionCall::ExpressionCall(const string &name) :
  Expression(),
  name(name),
  parameters() {

}

/* ------------------------------------------------------------------------- */

void ExpressionCall::addParameter(Co<Expression> parameter) {
  parameters.push_back(parameter);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionCall::evaluate(Environment &env) const {
  env.checkTimeout(getSourceLocation());

  Co<ExpressionClosure> closure = env[name];
  if (closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I couldn't find a function or variable named " + name + ".");
  }

  /* std::cout << "" << std::endl; */
  /* std::cout << "without names" << std::endl; */
  /* std::cout << "getSource(): " << getSource() << std::endl; */
  /* std::cout << "closure: " << closure << std::endl; */
  /* std::cout << "closure->GetDefine(): " << closure->getDefine() << std::endl; */
  /* std::cout << "closure->GetDefine()->name: " << closure->getDefine()->getName() << std::endl; */
  /* ExpressionArray *array2 = dynamic_cast<ExpressionArray *>(closure.GetPointer()); */
  /* std::cout << "array2: " << array2 << std::endl; */

  // I can't evaluate closure yet, because it may need params, right?

  Co<ExpressionDefine> define = closure->getDefine();

  // If the expression takes no parameters, it might be an array and we'll need
  // to look for a subscript. Even if its not, we can skip over a bunch of
  // logic for handling parameters.
  if (define->getArity() == 0) {
    Co<Expression> result = closure->evaluate(env);
    ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(result.GetPointer());
    if (array) {
      // assert only 1 param 
      if (parameters.size() != 1) {
        throw MessagedException(getSourceLocation().toAnchor() + ": " + "Bad subscript.");
      }

      // evaluate param
      Co<Expression> subscript_value = parameters[0]->evaluate(env);
      ExpressionInteger *subscript = dynamic_cast<ExpressionInteger *>(subscript_value.GetPointer());
      if (!subscript) {
        throw MessagedException(getSourceLocation().toAnchor() + ": " + "Non-integer subscript.");
      }

      return (*array->getArray())[subscript->toInteger()];
    } else {
      return result;
    }
  } else {
    // Make sure there are the correct number of parameters!
    if (parameters.size() != define->getArity()) {
      std::stringstream ss;
      ss << getSourceLocation().toAnchor() << ": I expect function " << define->getName() << " to be given " << define->getArity() << " parameter";
      if (define->getArity() != 1) {
        ss << "s";
      }

      if (define->getArity() > 0) {
        ss << ": " << define->getFormal(0).getName();
        for (unsigned int i = 1; i < define->getArity(); ++i) {
          ss << ", " << define->getFormal(i).getName();
        }
      }

      ss << ". But " << parameters.size() << (parameters.size() == 1 ? " was" : " were") << " given.";

      throw MessagedException(ss.str());
    }

    Co<Environment> closure_env(new Environment(*closure->getEnvironment()));
    for (unsigned int i = 0; i < define->getArity(); ++i) {
      const FormalParameter &formal = define->getFormal(i);
      bool is_lazy = formal.getEvaluationMode() == FormalParameter::LAZY;
      Co<ExpressionDefine> parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), is_lazy ? parameters[i] : parameters[i]->evaluate(env)));

      Co<ExpressionClosure> parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, is_lazy ? env : Environment()));
      parameter_closure->setSource(parameters[i]->getSource(), parameters[i]->getSourceLocation());

      closure_env->replace(define->getFormal(i).getName(), parameter_closure);
    }

    try {
      return closure->evaluate(*closure_env);
    } catch (UnlocatedException e) {
      throw MessagedException(getSourceLocation().toAnchor() + ": " + e.GetMessage());
    }
  }
}

/* ------------------------------------------------------------------------- */

void ExpressionCall::write(ostream &out) const {
  out << "(call " << name;
  for (vector<Co<Expression> >::const_iterator i = parameters.begin(); i != parameters.end(); ++i) {
    out << " " << *i;
  }
  out << ")";
}

/* ------------------------------------------------------------------------- */

const std::string &ExpressionCall::getName() const {
  return name;
}

/* ------------------------------------------------------------------------- */

int ExpressionCall::getArity() const {
  return parameters.size();
}

/* ------------------------------------------------------------------------- */

const Co<Expression> &ExpressionCall::operator[](int i) const {
  assert(0 <= i && i < parameters.size());
  return parameters[i]; 
}

/* ------------------------------------------------------------------------- */

}
