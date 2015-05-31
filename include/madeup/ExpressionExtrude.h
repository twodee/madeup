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

    Co<Expression> evaluate(Environment& env) const {
      /* float length = env.GetVariableAsFloat("length"); */
      /* float x = env.GetVariableAsFloat("x"); */
      /* float y = env.GetVariableAsFloat("y"); */
      /* float z = env.GetVariableAsFloat("z"); */

      Co<ExpressionClosure> closure = env["length"];
      if (closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function extrude expects a value named length. No value named length is defined.");
      }

      Co<Expression> v = closure->evaluate(env);
      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->getSourceLocation().toAnchor() + ": Function extrude expects length to be a number, but it is not.");
      }
      float length = number->toReal();

      closure = env["x"];
      if (closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function extrude expects a value named x. No value named x is defined.");
      }
      v = closure->evaluate(env);
      number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->getSourceLocation().toAnchor() + ": Function extrude expects x to be a number, but it is not.");
      }
      float x = number->toReal();

      closure = env["y"];
      if (closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function extrude expects a value named y. No value named y is defined.");
      }
      v = closure->evaluate(env);
      number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->getSourceLocation().toAnchor() + ": Function extrude expects y to be a number, but it is not.");
      }
      float y = number->toReal();

      closure = env["z"];
      if (closure.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function extrude expects a value named z. No value named z is defined.");
      }
      v = closure->evaluate(env);
      number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->getSourceLocation().toAnchor() + ": Function extrude expects z to be a number, but it is not.");
      }
      float z = number->toReal();

      QVector3<float> axis(x, y, z);
      env.extrude(axis, length);

      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(extrude)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
