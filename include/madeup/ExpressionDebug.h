#ifndef ELEMENTDEBUG_H
#define ELEMENTDEBUG_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionDebug : public Expression {
  public:
    ExpressionDebug();

    static bool print(const std::string &label,
                      const td::Co<Expression>& value);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
