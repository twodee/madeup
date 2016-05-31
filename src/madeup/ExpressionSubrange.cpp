#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionSubrange.h"
#include "madeup/ExpressionString.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionSubrange::ExpressionSubrange(Co<Expression> base_expression,
                                       Co<Expression> min_expression,
                                       Co<Expression> max_expression) :
  Expression(),
  base_expression(base_expression) {
  index_expressions[0] = min_expression;
  index_expressions[1] = max_expression;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubrange::evaluateBase(Environment &env) const {
  Co<Expression> base_value = base_expression->evaluate(env);
  return base_value;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionInteger> ExpressionSubrange::evaluateIndex(Environment &env,
                                                        int i) const {
  Co<Expression> index_value = index_expressions[i]->evaluate(env);

  ExpressionInteger *index = dynamic_cast<ExpressionInteger *>(index_value.GetPointer());

  if (!index) {
    throw MessagedException(index_expressions[i]->getSourceLocation().toAnchor() + ": I expect operator [] to be given an integer. " + index_expressions[i]->getSource() + " did not give me an integer.");
  }

  return Co<ExpressionInteger>(index_value);
}

/* ------------------------------------------------------------------------- */

Co<ExpressionInteger> ExpressionSubrange::evaluateIndex(Environment &env,
                                                        int i,
                                                        const ExpressionString *string_value) const {
  Co<ExpressionInteger> index_value = evaluateIndex(env, i);
  std::string s = string_value->getString();

  // Allow for negative indexing.
  if (index_value->toInteger() < 0) {
    index_value->setInteger(index_value->toInteger() + s.length());
  }

  if (index_value->toInteger() < 0 || index_value->toInteger() >= (int) s.length()) {
    std::stringstream ss;
    ss << index_expressions[i]->getSourceLocation().toAnchor();
    ss << ": I expect operator [] to be given valid indices. The string has ";
    ss << s.length();
    ss << " characters, indexed 0 through " << (s.length() - 1) << ". ";
    ss << index_expressions[i]->getSource() << " does not give me a valid index.";
    throw MessagedException(ss.str());
  }

  return index_value;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionInteger> ExpressionSubrange::evaluateIndex(Environment &env,
                                                        int i,
                                                        const ExpressionArrayReference *array_value) const {
  Co<ExpressionInteger> index_value = evaluateIndex(env, i);

  // Allow for negative indexing.
  if (index_value->toInteger() < 0) {
    index_value->setInteger(index_value->toInteger() + array_value->getArray()->getSize());
  }
  
  if (index_value->toInteger() < 0 || index_value->toInteger() >= array_value->getArray()->getSize()) {
    std::stringstream ss;
    ss << index_expressions[i]->getSourceLocation().toAnchor();
    ss << ": I expect operator [] to be given valid indices. The array has ";
    ss << array_value->getArray()->getSize();
    ss << " elements, indexed 0 through " << (array_value->getArray()->getSize() - 1) << ". ";
    ss << index_expressions[i]->getSource() << " does not give me a valid index.";
    throw MessagedException(ss.str());
  }

  return index_value;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubrange::evaluate(Environment &env) const {
  Co<Expression> base_value = evaluateBase(env);

  // Try as an array.
  ExpressionArrayReference *array_value = dynamic_cast<ExpressionArrayReference *>(base_value.GetPointer());
  if (array_value) {
    Co<ExpressionInteger> min_value = evaluateIndex(env, 0, array_value);
    Co<ExpressionInteger> max_value = evaluateIndex(env, 1, array_value);
    int imin = min_value->toInteger();
    int imax = max_value->toInteger();
    int n = imax - imin + 1;
    Co<ExpressionArray> new_array(new ExpressionArray(n >= 0 ? n : 0));
    for (int i = imin; i <= imax; ++i) {
      new_array->setElement(i - imin, (*array_value->getArray())[i]);
    }
    return Co<Expression>(new ExpressionArrayReference(new_array));
  }

  // Try as a string.
  ExpressionString *string_value = dynamic_cast<ExpressionString *>(base_value.GetPointer());
  if (string_value) {
    Co<ExpressionInteger> min_value = evaluateIndex(env, 0, string_value);
    Co<ExpressionInteger> max_value = evaluateIndex(env, 1, string_value);
    std::string s = string_value->getString();
    int imin = min_value->toInteger();
    int imax = max_value->toInteger();
    return Co<Expression>(new ExpressionString(s.substr(imin, imax - imin + 1)));
  }

  throw MessagedException(base_expression->getSourceLocation().toAnchor() + ": I expect operator [] to be applied to an array or a string. " + base_expression->getSource() + " is neither.");
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubrange::getBase() const {
  return base_expression;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubrange::getIndex(int i) const {
  return index_expressions[i];
}

/* ------------------------------------------------------------------------- */

void ExpressionSubrange::write(ostream &out) const {
  out << "(SUBRANGE ";
  base_expression->write(out);
  out << " ";
  index_expressions[0]->write(out);
  out << " ";
  index_expressions[1]->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
