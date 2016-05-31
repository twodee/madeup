#ifndef ELEMENTCLOSURE_H
#define ELEMENTCLOSURE_H

#include "madeup/Expression.h"
#include "madeup/ExpressionDefine.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionClosure : public Expression {
  public:
    ExpressionClosure(td::Co<ExpressionDefine> define);
    ExpressionClosure(td::Co<ExpressionDefine> define, td::Co<Environment> shared_env);
    ExpressionClosure(td::Co<ExpressionDefine> define, const Environment &env);

    td::Co<Expression> evaluate(Environment &env) const;

    td::Co<Environment> getEnvironment();
    void setEnvironment(const Environment &env);

    td::Co<ExpressionDefine> getDefine();
    void setDefine(td::Co<ExpressionDefine> define);

    void write(ostream &out) const;

  private:
    td::Co<ExpressionDefine> define;
    td::Co<Environment> env;
};

/* ------------------------------------------------------------------------- */

}

#endif
