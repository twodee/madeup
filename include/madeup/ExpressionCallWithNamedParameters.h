#ifndef EXPRESSIONCALLWITHNAMEDPARAMETERS_H
#define EXPRESSIONCALLWITHNAMEDPARAMETERS_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

typedef map<std::string, Co<Expression> > bindings_t;
typedef pair<std::string, Co<Expression> > binding_t;

/* ------------------------------------------------------------------------- */

class ExpressionCallWithNamedParameters : public Expression {
  public:
    ExpressionCallWithNamedParameters(const std::string &name);

    void addParameter(const std::string &name, Co<Expression> parameter);
    Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    std::string name;
    bindings_t bindings;
};

/* ------------------------------------------------------------------------- */

}

#endif
