#ifndef ELEMENTSURFACE_H
#define ELEMENTSURFACE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionSurface : public Expression {
  public:
    ExpressionSurface() :
      Expression() {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<ExpressionClosure> width = env["width"];
      Co<ExpressionClosure> height = env["height"];
      if (width.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function surface expects a value named width. No value named width is defined.");
      }
      if (height.IsNull()) {
        throw MessagedException(GetSourceLocation() + ": Function surface expects a value named height. No value named height is defined.");
      }
      Co<Expression> vwidth = width->Evaluate(env);
      Co<Expression> vheight = height->Evaluate(env);

      ExpressionInteger *viwidth = dynamic_cast<ExpressionInteger *>(vwidth.GetPointer());
      ExpressionInteger *viheight = dynamic_cast<ExpressionInteger *>(vheight.GetPointer());

      if (!viwidth) {
        throw MessagedException(width->GetSourceLocation() + ": Function surface expects an integer width. " + width->GetSource() + " is not an integer.");
      } else if (!viheight) {
        throw MessagedException(height->GetSourceLocation() + ": Function surface expects an integer height. " + height->GetSource() + " is not an integer.");
      }

      int iwidth = viwidth->GetInteger();
      int iheight = viheight->GetInteger();

      env.Surface(iwidth, iheight);

      return Co<Expression>(new ExpressionUnit());
    }

    void Write(ostream& out) const {
      out << "(surface width height)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
