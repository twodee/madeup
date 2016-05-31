#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionCall.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionInteger.h"

using std::string;
using std::vector;
using namespace td;

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

  Co<ExpressionDefine> define = closure->getDefine();

  // If the expression takes no parameters, it might be an array and we'll need
  // to look for a subscript. Even if its not, we can skip over a bunch of
  // logic for handling parameters.
  /* if (false && define->getArity() == 0) { */
    /* Co<Expression> result = closure->evaluate(env); */
    /* ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(result.GetPointer()); */
    /* if (array) { */
      /* // assert only 1 param  */
      /* if (parameters.size() != 1) { */
        /* throw MessagedException(getSourceLocation().toAnchor() + ": " + "Bad subscript."); */
      /* } */

      /* // evaluate param */
      /* Co<Expression> subscript_value = parameters[0]->evaluate(env); */
      /* ExpressionInteger *subscript = dynamic_cast<ExpressionInteger *>(subscript_value.GetPointer()); */
      /* if (!subscript) { */
        /* throw MessagedException(getSourceLocation().toAnchor() + ": " + "Non-integer subscript."); */
      /* } */

      /* return (*array->getArray())[subscript->toInteger()]; */
    /* } else { */
      /* return result; */
    /* } */
  /* } else { */

  unsigned int nactuals = 0;
  Co<Environment> closure_env(new Environment(*closure->getEnvironment()));
  for (unsigned int ai = 0, fi = 0; ai < parameters.size(); ++ai, ++fi) {
    const FormalParameter &formal = define->getFormal(fi);

    bool is_lazy = formal.getEvaluationMode() == FormalParameter::LAZY;

    if (is_lazy) {
      Co<ExpressionDefine> parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), parameters[ai]));
      Co<ExpressionClosure> parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, env));
      parameter_closure->setSource(parameters[ai]->getSource(), parameters[ai]->getSourceLocation());
      closure_env->replace(formal.getName(), parameter_closure);
      ++nactuals;
    }

    // Otherwise, let's eagerly evaluate.
    else {
      Co<Expression> actual = parameters[ai]->evaluate(env);

      // If this formal is splattable, let's see if we have an array that we can expand.
      bool isSplatted = false;
      if (formal.isSplattable()) {
        ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(actual.GetPointer());
        const vector<string> &splats = formal.getSplats();

        if (array) {
          for (int si = 0; si < array->getArray()->getSize(); ++si) {
            string id = splats[si];
            Co<ExpressionDefine> parameter_define = Co<ExpressionDefine>(new ExpressionDefine(id, (*array->getArray())[si]));
            Co<ExpressionClosure> parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, Environment()));
            parameter_closure->setSource(parameters[ai]->getSource(), parameters[ai]->getSourceLocation());
            closure_env->replace(id, parameter_closure);
          }

          // Advance iterator past the values we splatted.
          nactuals += array->getArray()->getSize();
          fi += array->getArray()->getSize() - 1;
          isSplatted = true;
        }
      }

      if (!isSplatted) {
        Co<ExpressionDefine> parameter_define = Co<ExpressionDefine>(new ExpressionDefine(formal.getName(), actual));
        Co<ExpressionClosure> parameter_closure = Co<ExpressionClosure>(new ExpressionClosure(parameter_define, Environment()));
        parameter_closure->setSource(parameters[ai]->getSource(), parameters[ai]->getSourceLocation());
        closure_env->replace(formal.getName(), parameter_closure);
        ++nactuals;
      }
    }
  }

  // Make sure there are the correct number of parameters!
  if (nactuals != define->getArity()) {
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

    ss << ". But " << nactuals << (nactuals == 1 ? " was" : " were") << " given.";

    throw MessagedException(ss.str());
  }

  try {
    return closure->evaluate(*closure_env);
  } catch (UnlocatedException e) {
    throw MessagedException(getSourceLocation().toAnchor() + ": " + e.GetMessage());
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

const Co<Expression> &ExpressionCall::operator[](unsigned int i) const {
  assert(0 <= i && i < parameters.size());
  return parameters[i]; 
}

/* ------------------------------------------------------------------------- */

}
