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

    Co<Expression> Evaluate(Environment& env) {
      return Co<Expression>((ExpressionArray *) (this));
    }

    int GetLength() {
      return nelements;
    }

    Co<Expression> operator[](int i) {
      if (i < 0 || i >= nelements) {
        throw MessagedException("ack!");
      }
      return elements[i];
    }

    void SetElement(int i, Co<Expression> expr) {
      elements[i] = expr;
    }

    void Write(ostream& out) const {
      out << "(ARRAY";
      for (int i = 0; i < nelements; ++i) {
        out << " ";
        elements[i]->Write(out);
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

    Co<Expression> Evaluate(Environment& env) {
      return Co<Expression>(new ExpressionArrayReference(array_expression));
    }

    void Write(ostream& out) const {
      out << "(ARRAYREF ";
      array_expression->Write(out);
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

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> length_value = length_expression->Evaluate(env);
      ExpressionInteger *length = dynamic_cast<ExpressionInteger *>(length_value.GetPointer());

      if (!length) {
        throw MessagedException(length_expression->GetSourceLocation().toAnchor() + ": An array expects its number of elements to be an integer. " + length_expression->GetSource() + " is not an integer.");
      }

      Co<ExpressionArray> array = Co<Expression>(new ExpressionArray(length->GetInteger(), Co<Expression>(new ExpressionUnit())));
      for (int i = 0; i < length->GetInteger(); ++i) {
        array->SetElement(i, fill_expression->Evaluate(env));
      }

      return Co<Expression>(new ExpressionArrayReference(array));
    }

    void Write(ostream& out) const {
      out << "(MAKEARRAY ";
      length_expression->Write(out);
      out << " ";
      fill_expression->Write(out);
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

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> array_value = array_expression->Evaluate(env);
      ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(array_value.GetPointer());

      if (!array) {
        throw MessagedException(array_expression->GetSourceLocation().toAnchor() + ": Length expects to be applied to an array. " + array_expression->GetSource() + " is not an array.");
      }

      return Co<Expression>(new ExpressionInteger(array->GetArray()->GetLength()));
    }

    void Write(ostream& out) const {
      out << "(LENGTH ";
      array_expression->Write(out);
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

    Co<ExpressionArrayReference> EvaluateArrayReference(Environment& env) {
      Co<Expression> array_value = array_expression->Evaluate(env);
      ExpressionArrayReference *array = dynamic_cast<ExpressionArrayReference *>(array_value.GetPointer());
      if (!array) {
        throw MessagedException(array_expression->GetSourceLocation().toAnchor() + ": Operator [] expects to be applied to an array. " + array_expression->GetSource() + " is not an array.");
      }

      return Co<ExpressionArrayReference>(array_value);
    }

    Co<ExpressionInteger> EvaluateIndex(Environment& env, Co<ExpressionArrayReference> array) {
      Co<Expression> index_value = index_expression->Evaluate(env);
      ExpressionInteger *index = dynamic_cast<ExpressionInteger *>(index_value.GetPointer());
      if (!index) {
        throw MessagedException(index_expression->GetSourceLocation().toAnchor() + ": Operator [] expects index to be an integer. " + index_expression->GetSource() + " is not an integer.");
      }

      if (index->GetInteger() < 0) {
        index->SetInteger(index->GetInteger() + array->GetArray()->GetLength());
      }

      if (index->GetInteger() < 0 || index->GetInteger() >= array->GetArray()->GetLength()) {
        std::stringstream ss;
        ss << index_expression->GetSourceLocation().toAnchor();
        ss << ": Operator [] expects a valid index. The array has ";
        ss << array->GetArray()->GetLength();
        ss << " elements, indexed 0 through " << (array->GetArray()->GetLength() - 1) << ". " << index_expression->GetSource() << " is not a valid index.";
        throw MessagedException(ss.str());
      }

      return Co<ExpressionInteger>(index_value);
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionArrayReference> array = EvaluateArrayReference(env);
      Co<ExpressionInteger> index = EvaluateIndex(env, array);
      return (*array->GetArray())[index->GetInteger()];
    }

    void Write(ostream& out) const {
      out << "(SUBSCRIPT ";
      array_expression->Write(out);
      out << " ";
      index_expression->Write(out);
      out << ")";
    }

  private:
    Co<Expression> array_expression;
    Co<Expression> index_expression;
};

/* ------------------------------------------------------------------------- */

}

#endif
