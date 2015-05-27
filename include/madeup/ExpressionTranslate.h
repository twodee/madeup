#ifndef ELEMENTTRANSLATE_H
#define ELEMENTTRANSLATE_H

#include "twodee/MessagedException.h"

#include "madeup/Expression.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDecimal.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionUnit.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionTranslate : public Expression {
  public:
    ExpressionTranslate() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> xyz[3] = {
        env["x"],
        env["y"],
        env["z"],
      };
      float v[3];

      if (xyz[0].IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function translate expects a value named x. No value named x is defined.");
      }

      if (xyz[1].IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function translate expects a value named y. No value named y is defined.");
      }

      if (xyz[2].IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function translate expects a value named z. No value named z is defined.");
      }

      for (int i = 0; i < 3; ++i) {
        Co<Expression> vv = xyz[i]->Evaluate(env);
        ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(vv.GetPointer());
        if (!number) {
          std::stringstream ss;
          ss << xyz[i]->GetSourceLocation() << ": Function translate expects a numeric " << (char) ('x' + i) << " coordinate. " << xyz[i]->GetSource() << " is not a number.";
          throw MessagedException(ss.str());
        }
        v[i] = number->GetDecimal();
      }

      env.Translate(v[0], v[1], v[2]);

      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(translate x y z)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
