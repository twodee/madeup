#ifndef ELEMENTBLOBS_H
#define ELEMENTBLOBS_H

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBlobs : public Expression {
  public:
    ExpressionBlobs() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> closure = env["grain"];
      if (closure.IsNull()) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": Function blobs expects a value named grain. No value named grain is defined.");
      }

      Co<Expression> v = closure->Evaluate(env);
      ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->GetSourceLocation().toAnchor() + ": Function blobs expects grain to be a number, but it is not.");
      }
      float grain = number->GetReal();

      closure = env["iso"];
      if (closure.IsNull()) {
        throw MessagedException(GetSourceLocation().toAnchor() + ": Function blob expects a value named iso. No value named iso is defined.");
      }
      v = closure->Evaluate(env);
      number = dynamic_cast<ExpressionNumber *>(v.GetPointer());
      if (!number) {
        throw MessagedException(v->GetSourceLocation().toAnchor() + ": Function blobs expects iso to be a number, but it is not.");
      }
      float iso = number->GetReal();

      env.Blobs(grain, iso);

      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(blobs)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
