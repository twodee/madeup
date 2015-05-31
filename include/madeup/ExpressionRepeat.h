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

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> r = Co<Expression>(new ExpressionUnit());

      Co<Expression> v = niterations->evaluate(env);
      ExpressionInteger *niterations_value = dynamic_cast<ExpressionInteger *>(v.GetPointer());
      if (!niterations_value) {
        throw MessagedException(niterations->getSourceLocation().toAnchor() + ": A repeat loop expects its number of iterations to be an integer. " + niterations->getSource() + " is not an integer.");
      }

      int n = niterations_value->toInteger();
      for (int i = 0; i < n; ++i) {
        r = body->evaluate(env);
      }

      return r;
    }

    void write(ostream& out) const {
      out << "(repeat ";
      niterations->write(out);
      out << " ";
      body->write(out);
      out << ")";
    }

  private:
    Co<Expression> niterations;
    Co<ExpressionBlock> body;
};

/* ------------------------------------------------------------------------- */

}

#endif
