#ifndef EXPRESSIONARRAY_H
#define EXPRESSIONARRAY_H

#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionArray : public Expression {
  public:
    ExpressionArray(int nelements, Co<Expression> fill) :
      Expression(),
      nelements(nelements) {
      elements = new Co<Expression>[nelements];
      for (int i = 0; i < nelements; ++i) {
        elements[i] = fill;
      }
    }

    Co<Expression> evaluate(Environment& env) const {
      return Co<Expression>((ExpressionArray *) (this));
    }

    int getLength() {
      return nelements;
    }

    Co<Expression> operator[](int i) {
      if (i < 0 || i >= nelements) {
        throw MessagedException("ack!");
      }
      return elements[i];
    }

    void setElement(int i, Co<Expression> expr) {
      elements[i] = expr;
    }

    void write(ostream& out) const {
      out << "(ARRAY";
      for (int i = 0; i < nelements; ++i) {
        out << " ";
        elements[i]->write(out);
      }
      out << ")";
    }

  private:
    Co<Expression> *elements;
    int nelements;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayReference : public Expression {
  public:
    ExpressionArrayReference(Co<ExpressionArray> array_expression) :
      array_expression(array_expression) {
    }

    Co<Expression> evaluate(Environment& env) const {
      return Co<Expression>(new ExpressionArrayReference(array_expression));
    }

    void write(ostream& out) const {
      out << "(ARRAYREF ";
      array_expression->write(out);
      out << ")";
    }

    Co<ExpressionArray> GetArray() {
      return array_expression;
    }

  private:
    Co<ExpressionArray> array_expression;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayConstructor : public Expression {
  public:
    ExpressionArrayConstructor(Co<Expression> fill_expression, Co<Expression> length_expression) :
      Expression(),
      fill_expression(fill_expression),
      length_expression(length_expression) {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> length_value = length_expression->evaluate(env);
      ExpressionInteger *length = dynamic_cast<ExpressionInteger *>(length_value.GetPointer());

      if (!length) {
        throw MessagedException(length_expression->getSourceLocation().toAnchor() + ": An array expects its number of elements to be an integer. " + length_expression->getSource() + " is not an integer.");
      }

      Co<ExpressionArray> array = Co<Expression>(new ExpressionArray(length->toInteger(), Co<Expression>(new ExpressionUnit())));
      for (int i = 0; i < length->toInteger(); ++i) {
        array->setElement(i, fill_expression->evaluate(env));
      }

      return Co<Expression>(new ExpressionArrayReference(array));
    }

    void write(ostream& out) const {
      out << "(MAKEARRAY ";
      length_expression->write(out);
      out << " ";
      fill_expression->write(out);
      out << ")";
    }

  private:
    Co<Expression> fill_expression;
    Co<Expression> length_expression;
};

/* ------------------------------------------------------------------------- */

class ExpressionArrayLength : public Expression {
  public:
    ExpressionArrayLength(Co<Expression> array_expression) :
      Expression(),
      array_expression(array_expression) {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> array_value = array_expression->evaluate(env);
      ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(array_value.GetPointer());

      if (!array) {
        throw MessagedException(array_expression->getSourceLocation().toAnchor() + ": Length expects to be applied to an array. " + array_expression->getSource() + " is not an array.");
      }

      return Co<Expression>(new ExpressionInteger(array->GetArray()->getLength()));
    }

    void write(ostream& out) const {
      out << "(LENGTH ";
      array_expression->write(out);
      out << ")";
    }

  private:
    Co<Expression> array_expression;
};

/* ------------------------------------------------------------------------- */

class ExpressionArraySubscript : public Expression {
  public:
    ExpressionArraySubscript(Co<Expression> array_expression, Co<Expression> index_expression) :
      Expression(),
      array_expression(array_expression),
      index_expression(index_expression) {
    }

    Co<ExpressionArrayReference> evaluateArrayReference(Environment& env) const {
      Co<Expression> array_value = array_expression->evaluate(env);
      ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(array_value.GetPointer());
      if (!array) {
        throw MessagedException(array_expression->getSourceLocation().toAnchor() + ": Operator [] expects to be applied to an array. " + array_expression->getSource() + " is not an array.");
      }

      return Co<ExpressionArrayReference>(array_value);
    }

    Co<ExpressionInteger> evaluateIndex(Environment& env, Co<ExpressionArrayReference> array) const {
      Co<Expression> index_value = index_expression->evaluate(env);
      ExpressionInteger *index = dynamic_cast<ExpressionInteger *>(index_value.GetPointer());
      if (!index) {
        throw MessagedException(index_expression->getSourceLocation().toAnchor() + ": Operator [] expects index to be an integer. " + index_expression->getSource() + " is not an integer.");
      }

      if (index->toInteger() < 0) {
        index->setInteger(index->toInteger() + array->GetArray()->getLength());
      }

      if (index->toInteger() < 0 || index->toInteger() >= array->GetArray()->getLength()) {
        std::stringstream ss;
        ss << index_expression->getSourceLocation().toAnchor();
        ss << ": Operator [] expects a valid index. The array has ";
        ss << array->GetArray()->getLength();
        ss << " elements, indexed 0 through " << (array->GetArray()->getLength() - 1) << ". " << index_expression->getSource() << " is not a valid index.";
        throw MessagedException(ss.str());
      }

      return Co<ExpressionInteger>(index_value);
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionArrayReference> array = evaluateArrayReference(env);
      Co<ExpressionInteger> index = evaluateIndex(env, array);
      return (*array->GetArray())[index->toInteger()];
    }

    void write(ostream& out) const {
      out << "(SUBSCRIPT ";
      array_expression->write(out);
      out << " ";
      index_expression->write(out);
      out << ")";
    }

  private:
    Co<Expression> array_expression;
    Co<Expression> index_expression;
};

/* ------------------------------------------------------------------------- */

}

#endif
