#ifndef EXPRESSIONNODES_H
#define EXPRESSIONNODES_H

#include "madeup/Expression.h"
#include "twodee/Trimesh.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNodes : public Expression {
  public:
    ExpressionNodes(std::vector<Node> path);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;
    const std::vector<Node> &getPath() const;

  private:
    std::vector<Node> path;
};

/* ------------------------------------------------------------------------- */

}

#endif
