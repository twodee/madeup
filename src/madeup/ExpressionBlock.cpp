#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefineElement.h"
#include "madeup/ExpressionDefineVariable.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionUnit.h"

using std::vector;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionBlock::ExpressionBlock() :
  Expression(),
  statements() {
}

/* ------------------------------------------------------------------------- */

void ExpressionBlock::append(Co<Expression> element) {
  statements.push_back(element);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBlock::evaluate(Environment &env) const {
  for (vector<Co<Expression> >::const_iterator i = statements.begin(); i != statements.end(); ++i) {
    (*i)->predeclare(env);
  }

  Co<Expression> value(ExpressionUnit::getSingleton());
  int iexpr = 0;
  for (vector<Co<Expression> >::const_iterator i = statements.begin(); i != statements.end(); ++i, ++iexpr) {
    value = (*i)->evaluate(env);

    // Don't issue geometry if expression was an assignment.
    if ((*i)->isEmitter()) {
      ExpressionMesh *mesh = dynamic_cast<ExpressionMesh *>(value.GetPointer());
      if (mesh) {
        env.echoWithoutTransform(mesh->toMesh());

        // Disable return of mesh when last expression is a solidifier. We want to avoid this:
        //
        // to foo
        //   moveto 0, 0, 0
        //   box
        // end
        //
        // foo
        // 
        // This would emit double geometry, once on the box call, once on the foo.
        if (iexpr == getLength() - 1) {
          value = ExpressionUnit::getSingleton();
        }
      } else {
        ExpressionNodes *nodes = dynamic_cast<ExpressionNodes *>(value.GetPointer());
        if (nodes) {
          env.echoWithoutTransform(nodes->getPath());
          if (iexpr == getLength() - 1) {
            value = ExpressionUnit::getSingleton();
          }
        }
      }
    }
  }

  return value;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBlock::operator[](size_t i) {
  assert(0 <= i && i < statements.size());
  return statements[i];
}

/* ------------------------------------------------------------------------- */

int ExpressionBlock::getLength() const {
  return statements.size();
}

/* ------------------------------------------------------------------------- */

void ExpressionBlock::write(ostream &out) const {
  out << "(block";
  for (vector<Co<Expression> >::const_iterator i = statements.begin(); i != statements.end(); ++i) {
    out << " ";
    (*i)->write(out);
  }
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
