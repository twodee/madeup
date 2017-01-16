#ifndef MESHBOOLEAN_H
#define MESHBOOLEAN_H

#include <madeup/Environment.h>
#include <madeup/ExpressionMesh.h>
#include <twodee/Trimesh.h>

namespace madeup {
namespace MeshBoolean {

/* ------------------------------------------------------------------------- */

enum operation_t {
  UNION,
  DIFFERENCE,
  INTERSECTION
};

td::Trimesh *construct(const td::Trimesh &a,
                       const td::Trimesh &b,
                       operation_t operation);

td::Co<Expression> construct_and_color(ExpressionMesh *l_mesh,
                                       ExpressionMesh *r_mesh,
                                       Environment &env,
                                       operation_t operation);
td::Trimesh *construct_and_color(const td::Trimesh &l_mesh,
                                 const td::Trimesh &r_mesh,
                                 Environment &env,
                                 operation_t operation);

/* ------------------------------------------------------------------------- */

td::Trimesh *compute_normals(const td::Trimesh &mesh, float angle);

}
}

#endif
