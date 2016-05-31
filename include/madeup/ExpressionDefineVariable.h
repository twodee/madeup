#ifndef ELEMENTDEFINEVARIABLE_H
#define ELEMENTDEFINEVARIABLE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDefineVariable : public Expression {
  public:
    ExpressionDefineVariable(const std::string &name, td::Co<Expression> rhs);
    td::Co<Expression> evaluate(Environment &env) const;
    td::Co<Expression> getRightHandSide() const;
    void write(ostream &out) const;
    const std::string &getName() { return name; }

  protected:
    std::string name;
    td::Co<Expression> rhs;
};

/* ------------------------------------------------------------------------- */

class ExpressionDefineVariableSeed : public ExpressionDefineVariable {
  public:
    ExpressionDefineVariableSeed(const std::string &name, td::Co<Expression> rhs);
    td::Co<Expression> evaluate(Environment &env) const;

  private:
};

/* ------------------------------------------------------------------------- */

/* class ExpressionDefineVariableRadius : public ExpressionDefineVariable { */
  /* public: */
    /* ExpressionDefineVariableRadius(const std::string &name, td::Co<Expression> rhs); */
    /* td::Co<Expression> evaluate(Environment &env) const; */

  /* private: */
/* }; */

/* ------------------------------------------------------------------------- */

}

#endif
