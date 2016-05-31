#ifndef ELEMENTCALL_H
#define ELEMENTCALL_H

#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCall : public Expression {
  public:
    ExpressionCall(const std::string &name);

    void addParameter(td::Co<Expression> parameter);
    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;
    const std::string &getName() const;
    int getArity() const;
    const td::Co<Expression> &operator[](unsigned int i) const;

  private:
    std::string name;
    std::vector<td::Co<Expression> > parameters;
};

/* ------------------------------------------------------------------------- */

}

#endif
