#ifndef EXPRESSIONMESH_H
#define EXPRESSIONMESH_H

#include "madeup/Expression.h"
#include "twodee/Trimesh.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionMesh : public Expression {
  public:
    ExpressionMesh(Co<Trimesh> mesh);

    Co<Expression> evaluate(Environment &env) const;
    Co<Trimesh> toMesh() const;
    void write(ostream &out) const;

  private:
    Co<Trimesh> mesh;
};

/* ------------------------------------------------------------------------- */

}

#endif
