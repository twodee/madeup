#ifndef ELEMENTCLOSURE_H
#define ELEMENTCLOSURE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionDefine.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionClosure : public Expression {
  public:
    ExpressionClosure(Co<ExpressionDefine> define);
    ExpressionClosure(Co<ExpressionDefine> define, Co<Environment> shared_env);
    ExpressionClosure(Co<ExpressionDefine> define, const Environment &env);

    Co<Expression> evaluate(Environment &env) const;

    Co<Environment> getEnvironment();
    void setEnvironment(const Environment &env);

    Co<ExpressionDefine> getDefine();
    void setDefine(Co<ExpressionDefine> define);

    void write(ostream &out) const;

  private:
    Co<ExpressionDefine> define;
    Co<Environment> env;
};

/* ------------------------------------------------------------------------- */

}

#endif
