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

    void Append(Co<Expression> element) {
      statements.push_back(element);
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> value(new ExpressionUnit());
      for (vector<Co<Expression> >::iterator i = statements.begin(); i != statements.end(); ++i) {
        value = (*i)->Evaluate(env);
      }
      return value;
    }

    Co<Expression> operator[](int i) {
      assert(0 <= i && i < statements.size());
      return statements[i];
    }

    int GetLength() const {
      return statements.size();
    }

    void Write(ostream& out) const {
      out << "(block ";
      vector<Co<Expression> >::const_iterator i = statements.begin();
      if (i != statements.end()) {
        (*i)->Write(out);
        for (++i; i != statements.end(); ++i) {
          out << " ";
          (*i)->Write(out);
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
