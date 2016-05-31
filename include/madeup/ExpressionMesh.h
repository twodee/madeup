#ifndef EXPRESSIONMESH_H
#define EXPRESSIONMESH_H

#include "madeup/Expression.h"
#include "twodee/Trimesh.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMesh : public Expression {
  public:
    ExpressionMesh(td::Co<td::Trimesh> mesh);

    td::Co<Expression> evaluate(Environment &env) const;
    td::Co<td::Trimesh> toMesh() const;
    void write(ostream &out) const;

  private:
    td::Co<td::Trimesh> mesh;
};

/* ------------------------------------------------------------------------- */

}

#endif
