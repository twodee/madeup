#ifndef ELEMENTREPEAT_H
#define ELEMENTREPEAT_H

#include "madeup/Expression.h"
#include "twodee/MessagedException.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionInteger.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionRepeat : public Expression {
  public:
    ExpressionRepeat(Co<Expression> niterations, Co<ExpressionBlock> body) :
      Expression(),
      niterations(niterations),
      body(body) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> r = Co<Expression>(new ExpressionUnit());

      Co<Expression> v = niterations->Evaluate(env);
      ExpressionInteger *niterations_value = dynamic_cast<ExpressionInteger *>(v.GetPointer());
      if (!niterations_value) {
        throw MessagedException(niterations->GetSourceLocation() + ": A repeat loop expects its number of iterations to be an integer. " + niterations->GetSource() + " is not an integer.");
      }

      int n = niterations_value->GetInteger();
      for (int i = 0; i < n; ++i) {
        r = body->Evaluate(env);
      }

      return r;
    }

    void Write(ostream& out) const {
      out << "(repeat ";
      niterations->Write(out);
      out << " ";
      body->Write(out);
      out << ")";
    }

  private:
    Co<Expression> niterations;
    Co<ExpressionBlock> body;
};

/* ------------------------------------------------------------------------- */

}

#endif
