#ifndef EXPRESSIONBLOCK_H
#define EXPRESSIONBLOCK_H

#include <vector>

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionBlock : public Expression {
  public:
    ExpressionBlock() :
      Expression(),
      elements() {
    }

    void AddExpression(Co<Expression> element) {
      elements.push_back(element);
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> value(new ExpressionUnit());
      for (vector<Co<Expression> >::iterator i = elements.begin(); i != elements.end(); ++i) {
        std::cerr << "evaluating " << (*i)->GetSource() << std::endl;
        value = (*i)->Evaluate(env);
      }
      return value;
    }

    void Write(ostream& out) const {
      out << "(block ";
      vector<Co<Expression> >::const_iterator i = elements.begin();
      if (i != elements.end()) {
        (*i)->Write(out);
        for (++i; i != elements.end(); ++i) {
          out << " ";
          (*i)->Write(out);
        }
      }
      out << ")";
    }

  private:
    vector<Co<Expression> > elements;
};

/* ------------------------------------------------------------------------- */

}

#endif
