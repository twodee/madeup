#ifndef ELEMENTIDENTIFIER_H
#define ELEMENTIDENTIFIER_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionIdentifier : public Expression {
  public:
    ExpressionIdentifier(const std::string &name);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;
    const std::string &getName() const;

  private:
    std::string name;
};

/* ------------------------------------------------------------------------- */

}

#endif
