#ifndef ELEMENTCLOSURE_H
#define ELEMENTCLOSURE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionDefine.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionClosure : public Expression {
  public:
    ExpressionClosure(Co<ExpressionDefine> define, const Environment& env);

    Co<Expression> Evaluate(Environment& env);

    Co<Environment> GetEnvironment();
    void SetEnvironment(const Environment& env);

    Co<ExpressionDefine> GetDefine();
    void SetDefine(Co<ExpressionDefine> define);

    void Write(ostream& out) const;

  private:
    Co<ExpressionDefine> define;
    Co<Environment> env;
};

/* ------------------------------------------------------------------------- */

}

#endif
