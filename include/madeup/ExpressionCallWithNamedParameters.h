#ifndef EXPRESSIONCALLWITHNAMEDPARAMETERS_H
#define EXPRESSIONCALLWITHNAMEDPARAMETERS_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

typedef map<std::string, td::Co<Expression> > bindings_t;
typedef pair<std::string, td::Co<Expression> > binding_t;

/* ------------------------------------------------------------------------- */

class ExpressionCallWithNamedParameters : public Expression {
  public:
    ExpressionCallWithNamedParameters(const std::string &name);

    void addParameter(const std::string &name, td::Co<Expression> parameter);
    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    std::string name;
    bindings_t bindings;
};

/* ------------------------------------------------------------------------- */

}

#endif
