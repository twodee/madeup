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
      statements() {
    }

    void append(Co<Expression> element) {
      statements.push_back(element);
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> value(new ExpressionUnit());
      for (vector<Co<Expression> >::const_iterator i = statements.begin(); i != statements.end(); ++i) {
        value = (*i)->evaluate(env);
      }
      return value;
    }

    Co<Expression> operator[](int i) {
      assert(0 <= i && i < statements.size());
      return statements[i];
    }

    int getLength() const {
      return statements.size();
    }

    void write(ostream& out) const {
      out << "(block ";
      vector<Co<Expression> >::const_iterator i = statements.begin();
      if (i != statements.end()) {
        (*i)->write(out);
        for (++i; i != statements.end(); ++i) {
          out << " ";
          (*i)->write(out);
        }
      }
      out << ")";
    }

  private:
    vector<Co<Expression> > statements;
};

/* ------------------------------------------------------------------------- */

}

#endif
