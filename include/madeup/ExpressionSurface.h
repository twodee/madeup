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

    Co<Expression> evaluate(Environment& env) const {
      Co<ExpressionClosure> width = env["width"];
      Co<ExpressionClosure> height = env["height"];
      if (width.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function surface expects a value named width. No value named width is defined.");
      }
      if (height.IsNull()) {
        throw MessagedException(getSourceLocation().toAnchor() + ": Function surface expects a value named height. No value named height is defined.");
      }
      Co<Expression> vwidth = width->evaluate(env);
      Co<Expression> vheight = height->evaluate(env);

      ExpressionInteger *viwidth = dynamic_cast<ExpressionInteger *>(vwidth.GetPointer());
      ExpressionInteger *viheight = dynamic_cast<ExpressionInteger *>(vheight.GetPointer());

      if (!viwidth) {
        throw MessagedException(width->getSourceLocation().toAnchor() + ": Function surface expects an integer width. " + width->getSource() + " is not an integer.");
      } else if (!viheight) {
        throw MessagedException(height->getSourceLocation().toAnchor() + ": Function surface expects an integer height. " + height->getSource() + " is not an integer.");
      }

      int iwidth = viwidth->toInteger();
      int iheight = viheight->toInteger();

      env.surface(iwidth, iheight);

      return Co<Expression>(new ExpressionUnit());
    }

    void write(ostream& out) const {
      out << "(surface width height)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
