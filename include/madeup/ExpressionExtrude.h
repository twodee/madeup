#ifndef ELEMENTEXTRUDE_H
#define ELEMENTEXTRUDE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionExtrude : public Expression {
  public:
    ExpressionExtrude() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      /* float length = env.GetVariableAsFloat("length"); */
      /* float x = env.GetVariableAsFloat("x"); */
      /* float y = env.GetVariableAsFloat("y"); */
      /* float z = env.GetVariableAsFloat("z"); */

      Co<ExpressionClosure> closure = env["length"];
      if (closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function extrude expects a value named length. No value named length is defined.");
      }

      Co<Expression> v = closure->Evaluate(env);
      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->GetSourceLocation() + ": Function extrude expects length to be a number, but it is not.");
      }
      float length = number->GetDecimal();

      closure = env["x"];
      if (closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function extrude expects a value named x. No value named x is defined.");
      }
      v = closure->Evaluate(env);
      number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->GetSourceLocation() + ": Function extrude expects x to be a number, but it is not.");
      }
      float x = number->GetDecimal();

      closure = env["y"];
      if (closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function extrude expects a value named y. No value named y is defined.");
      }
      v = closure->Evaluate(env);
      number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->GetSourceLocation() + ": Function extrude expects y to be a number, but it is not.");
      }
      float y = number->GetDecimal();

      closure = env["z"];
      if (closure.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function extrude expects a value named z. No value named z is defined.");
      }
      v = closure->Evaluate(env);
      number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->GetSourceLocation() + ": Function extrude expects z to be a number, but it is not.");
      }
      float z = number->GetDecimal();

      QVector3<float> axis(x, y, z);
      env.Extrude(axis, length);

      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(extrude)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
