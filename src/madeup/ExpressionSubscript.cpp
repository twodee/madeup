#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionSubscript.h"
#include "madeup/ExpressionString.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionSubscript::ExpressionSubscript(Co<Expression> base_expression,
                                         Co<Expression> index_expression) :
  Expression(),
  base_expression(base_expression),
  index_expression(index_expression) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubscript::evaluateBase(Environment &env) const {
  Co<Expression> base_value = base_expression->evaluate(env);
  return base_value;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionInteger> ExpressionSubscript::evaluateIndex(Environment &env) const {
  Co<Expression> index_value = index_expression->evaluate(env);

  ExpressionInteger *index = dynamic_cast<ExpressionInteger *>(index_value.GetPointer());

  if (!index) {
    throw MessagedException(index_expression->getSourceLocation().toAnchor() + ": I expect operator [] to be given an integer. " + index_expression->getSource() + " did not give me an integer.");
  }

  return Co<ExpressionInteger>(index_value);
}

/* ------------------------------------------------------------------------- */

Co<ExpressionInteger> ExpressionSubscript::evaluateIndex(Environment &env,
                                                         const ExpressionString *string_value) const {
  Co<ExpressionInteger> index_value = evaluateIndex(env);
  std::string s = string_value->getString();

  // Allow for negative indexing.
  if (index_value->toInteger() < 0) {
    index_value->setInteger(index_value->toInteger() + s.length());
  }

  if (index_value->toInteger() < 0 || index_value->toInteger() >= s.length()) {
    std::stringstream ss;
    ss << index_expression->getSourceLocation().toAnchor();
    ss << ": I expect operator [] to be given a valid index. The string has ";
    ss << s.length();
    ss << " characters, indexed 0 through " << (s.length() - 1) << ". ";
    ss << index_expression->getSource() << " does not give me a valid index.";
    throw MessagedException(ss.str());
  }

  return index_value;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionInteger> ExpressionSubscript::evaluateIndex(Environment &env,
                                                         const ExpressionArrayReference *array_value) const {
  Co<ExpressionInteger> index_value = evaluateIndex(env);

  // Allow for negative indexing.
  if (index_value->toInteger() < 0) {
    index_value->setInteger(index_value->toInteger() + array_value->getArray()->getSize());
  }
  
  if (index_value->toInteger() < 0 || index_value->toInteger() >= array_value->getArray()->getSize()) {
    std::stringstream ss;
    ss << index_expression->getSourceLocation().toAnchor();
    ss << ": I expect operator [] to be given a valid index. The array has ";
    ss << array_value->getArray()->getSize();
    ss << " elements, indexed 0 through " << (array_value->getArray()->getSize() - 1) << ". ";
    ss << index_expression->getSource() << " does not give me a valid index.";
    throw MessagedException(ss.str());
  }

  return index_value;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubscript::evaluate(Environment &env) const {
  Co<Expression> base_value = evaluateBase(env);

  // Try as an array.
  ExpressionArrayReference *array_value = dynamic_cast<ExpressionArrayReference *>(base_value.GetPointer());
  if (array_value) {
    Co<ExpressionInteger> index_value = evaluateIndex(env, array_value);
    return (*array_value->getArray())[index_value->toInteger()];
  }

  // Try as a string.
  ExpressionString *string_value = dynamic_cast<ExpressionString *>(base_value.GetPointer());
  if (string_value) {
    Co<ExpressionInteger> index_value = evaluateIndex(env, string_value);
    std::string s = string_value->getString();
    return Co<Expression>(new ExpressionString(std::string() + s[index_value->toInteger()]));
  }

  throw MessagedException(base_expression->getSourceLocation().toAnchor() + ": I expect operator [] to be applied to an array or a string. " + base_expression->getSource() + " is neither.");
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubscript::getBase() const {
  return base_expression;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubscript::getIndex() const {
  return index_expression;
}

/* ------------------------------------------------------------------------- */

void ExpressionSubscript::write(ostream &out) const {
  out << "(SUBSCRIPT ";
  base_expression->write(out);
  out << " ";
  index_expression->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
