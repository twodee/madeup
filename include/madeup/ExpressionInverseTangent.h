#ifndef ELEMENTINVERSETANGENT_H
#define ELEMENTINVERSETANGENT_H

#include <math.h>

#include "madeup/Expression.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionNumber.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionInverseTangent : public Expression {
  public:
    ExpressionInverseTangent() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> ratio_closure = env["ratio"];
      if (ratio_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function atan expects a value named ratio. No value named ratio is defined.");
      }

      Co<Expression> v = ratio_closure->Evaluate(env);

      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (number) {
        float value = number->GetReal();
        return Co<Expression>(new ExpressionReal(atanf(value) * 180.0f / td::PI));
      }

      throw MessagedException(ratio_closure->GetSourceLocation() + ": Function atan expects a number. " + ratio_closure->GetSource() + " is not a number.");
    }

    void Write(ostream& out) const {
      out << "(atan x)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

class ExpressionInverseTangent2 : public Expression {
  public:
    ExpressionInverseTangent2() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      // Opposite
      Co<ExpressionClosure> opposite_closure = env["opposite"];
      if (opposite_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function atan2 expects a value named opposite. No value named opposite is defined.");
      }

      Co<Expression> v = opposite_closure->Evaluate(env);

      ExpressionNumber *opposite = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!opposite) {
        throw MessagedException(opposite_closure->GetSourceLocation() + ": Function atan2 expects opposite to be a number. " + opposite_closure->GetSource() + " is not a number.");
      }

      float opposite_value = opposite->GetReal();

      // Adjacent
      Co<ExpressionClosure> adjacent_closure = env["adjacent"];
      if (adjacent_closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function atan2 expects a value named adjacent. No value named adjacent is defined.");
      }

      v = adjacent_closure->Evaluate(env);
      
      ExpressionNumber *adjacent = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!adjacent) {
        throw MessagedException(adjacent_closure->GetSourceLocation() + ": Function atan2 expects adjacent to be a number. " + adjacent_closure->GetSource() + " is not a number.");
      }

      float adjacent_value = adjacent->GetReal();

      return Co<Expression>(new ExpressionReal(atan2f(opposite_value, adjacent_value) * 180.0f / td::PI));
    }

    void Write(ostream& out) const {
      out << "(atan2 opposite adjacent)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
