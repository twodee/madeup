#ifndef ELEMENTDEFINEVARIABLE_H
#define ELEMENTDEFINEVARIABLE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionUnit.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDefineVariable : public Expression {
  public:
    ExpressionDefineVariable(const std::string& name, Co<Expression> rhs);
    Co<Expression> Evaluate(Environment& env);
    Co<Expression> GetRightHandSide() const;
    void Write(ostream& out) const;
    const std::string& GetName() { return name; }

  private:
    std::string name;
    Co<Expression> rhs;
};

/* ------------------------------------------------------------------------- */

class ExpressionDefineVariableSeed : public ExpressionDefineVariable {
  public:
    ExpressionDefineVariableSeed(const std::string& name, Co<Expression> rhs);
    Co<Expression> Evaluate(Environment& env);

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
