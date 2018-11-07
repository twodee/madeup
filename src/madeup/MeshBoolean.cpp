#include <iostream>

#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
#define IGL_NO_CORK
#include <igl/copyleft/cgal/wire_mesh.h>
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <igl/per_corner_normals.h>

#include <madeup/ExpressionArray.h>
#include <madeup/ExpressionClosure.h>
#include <madeup/ExpressionNumber.h>
#include <madeup/MeshBoolean.h>

using namespace td;

namespace madeup {
namespace MeshBoolean {

/* ------------------------------------------------------------------------- */

void trimeshToEigen(const Trimesh &mesh,
                    Eigen::MatrixXd &vertices,
                    Eigen::MatrixXi &faces) {
  vertices.resize(mesh.GetVertexCount(), 3);
  faces.resize(mesh.GetFaceCount(), 3);

  const float *position = mesh.GetPositions();
  for (int i = 0; i < mesh.GetVertexCount(); ++i) {
    vertices(i, 0) = position[0];
    vertices(i, 1) = position[1];
    vertices(i, 2) = position[2];
    position += 3;
  }

  const int *face = mesh.GetFaces();
  for (int i = 0; i < mesh.GetFaceCount(); ++i) {
    faces(i, 0) = face[0];
    faces(i, 1) = face[1];
    faces(i, 2) = face[2];
    face += 3;
  }
}

/* ------------------------------------------------------------------------- */

Trimesh *eigenToTrimesh(const Eigen::MatrixXd &vertices,
                        const Eigen::MatrixXi &faces) {
  Trimesh *join = new Trimesh(vertices.rows(), faces.rows());

  float *position = join->GetPositions();
  for (int i = 0; i < vertices.rows(); ++i) {
    position[0] = vertices(i, 0);
    position[1] = vertices(i, 1);
    position[2] = vertices(i, 2);
    position += 3;
  }

  int *face = join->GetFaces();
  for (int i = 0; i < faces.rows(); ++i) {
    face[0] = faces(i, 0);
    face[1] = faces(i, 1);
    face[2] = faces(i, 2);
    face += 3;
  }

  return join;
}

/* ------------------------------------------------------------------------- */

Trimesh *eigenToTrimesh(const float *vertex_colors,
                        const Eigen::MatrixXd &vertices,
                        const Eigen::MatrixXi &faces,
                        const Eigen::MatrixXd &normals) {
  Trimesh *join = new Trimesh(faces.rows() * 3, faces.rows());

  int *face = join->GetFaces();
  float *position = join->GetPositions();
  float *normal = join->AllocateNormals();
  float *color = join->AllocateVertexColors();

  for (int fi = 0; fi < faces.rows(); ++fi) {
    for (int ti = 0; ti < 3; ++ti) {
      int vi = faces(fi, ti);

      position[0] = vertices(vi, 0);
      position[1] = vertices(vi, 1);
      position[2] = vertices(vi, 2);
      position += 3;

      color[0] = vertex_colors[vi * 3 + 0];
      color[1] = vertex_colors[vi * 3 + 1];
      color[2] = vertex_colors[vi * 3 + 2];
      color += 3;

      normal[0] = normals(fi * 3 + ti, 0);
      normal[1] = normals(fi * 3 + ti, 1);
      normal[2] = normals(fi * 3 + ti, 2);
      normal += 3;
    }

    face[0] = fi * 3 + 0;
    face[1] = fi * 3 + 1;
    face[2] = fi * 3 + 2;
    face += 3;
  }

  /* for (int i = 0; i < faces.rows(); ++i) { */
    /* face[0] = faces(i, 0); */
    /* face[1] = faces(i, 1); */
    /* face[2] = faces(i, 2); */
    /* face += 3; */
  /* } */

  return join;
}

/* ------------------------------------------------------------------------- */

Trimesh *construct(const Trimesh &a,
                   const Trimesh &b,
                   operation_t operation) {

  igl::MeshBooleanType boolean_operation;
  switch (operation) {
    case UNION:
      boolean_operation = igl::MESH_BOOLEAN_TYPE_UNION;
      break;
    case DIFFERENCE:
      boolean_operation = igl::MESH_BOOLEAN_TYPE_MINUS;
      break;
    case INTERSECTION:
      boolean_operation = igl::MESH_BOOLEAN_TYPE_INTERSECT;
      break;
  }

  Eigen::MatrixXd vertices[3];
  Eigen::MatrixXi faces[3];

  trimeshToEigen(a, vertices[0], faces[0]);
  trimeshToEigen(b, vertices[1], faces[1]);

  Eigen::VectorXi J;
  igl::copyleft::cgal::mesh_boolean(vertices[0], faces[0], vertices[1], faces[1], boolean_operation, vertices[2], faces[2], J);

  /* Eigen::MatrixXd N_corners; */
  /* igl::per_corner_normals(vertices[2], faces[2], 20, N_corners); */

  return eigenToTrimesh(vertices[2], faces[2]);
}

/* ------------------------------------------------------------------------- */

Co<Expression> construct_and_color(ExpressionMesh *l_mesh,
                                   ExpressionMesh *r_mesh,
                                   Environment &env,
                                   operation_t operation) {
  Co<Trimesh> l = l_mesh->toMesh();
  Co<Trimesh> r = r_mesh->toMesh();
  return Co<Expression>(new ExpressionMesh(construct_and_color(*l, *r, env, operation)));
}

/* ------------------------------------------------------------------------- */

td::Trimesh *construct_and_color(const Trimesh &l,
                                 const Trimesh &r,
                                 Environment &env,
                                 operation_t operation) {

  // If the meshes are both empty, libigl will complain. Let's preempt
  // that. Empty meshes come in when we're not in SURFACE mode or when
  // the user just doesn't visit any vertices before solidifying.
  if (l.GetVertexCount() == 0 && r.GetVertexCount() == 0) {
    Trimesh *trimesh = new Trimesh(0, 0);
    trimesh->AllocateVertexColors();
    return trimesh;
  }

  Trimesh *diff = MeshBoolean::construct(l, r, operation);

  Co<Expression> v = env[".rgb"]->evaluate(env);
  ExpressionArrayReference *rgb_value = dynamic_cast<ExpressionArrayReference *>(v.GetPointer());
  QVector3<float> rgb;
  if (rgb_value) {
    Co<ExpressionArray> array = rgb_value->getArray();
    for (int i = 0; i < 3; ++i) {
      Co<Expression> element_value = (*array)[i];
      ExpressionNumber *element_number = dynamic_cast<ExpressionNumber *>(element_value.GetPointer());
      rgb[i] = element_number->toReal();
    }
  } else {
    rgb = QVector3<float>(1.0f, 0.0f, 0.0f);
  }

  float *colors = diff->AllocateVertexColors();
  float *color = colors;
  for (int vi = 0; vi < diff->GetVertexCount(); ++vi) {
    color[0] = rgb[0];
    color[1] = rgb[1];
    color[2] = rgb[2];
    color += 3;
  }

  return diff;
}

/* ------------------------------------------------------------------------ */

td::Trimesh *compute_normals(const td::Trimesh &mesh, float sharp_degrees) {
  // First prep the mesh to be used by IGL.
  Eigen::MatrixXd vertices;
  Eigen::MatrixXi faces;
  trimeshToEigen(mesh, vertices, faces);

  // Generate new normals.
  Eigen::MatrixXd normals;
  igl::per_corner_normals(vertices, faces, sharp_degrees, normals);

  td::Trimesh *mesh_with_normals = eigenToTrimesh(mesh.GetColors(), vertices, faces, normals);

  return mesh_with_normals;
}

/* ------------------------------------------------------------------------- */

td::Trimesh *network(const vector<QVector3<float>> &positions,
                     const vector<QVector2<int>> &edges,
                     float radius,
                     int nsides) {
  
  Eigen::MatrixXd eigen_vertices(positions.size(), 3);
  Eigen::MatrixXi eigen_edges(edges.size(), 2);

  for (int i = 0; i < positions.size(); ++i) {
    eigen_vertices(i, 0) = positions[i][0];
    eigen_vertices(i, 1) = positions[i][1];
    eigen_vertices(i, 2) = positions[i][2];
  }

  for (int i = 0; i < edges.size(); ++i) {
    eigen_edges(i, 0) = edges[i][0];
    eigen_edges(i, 1) = edges[i][1];
  }

  Eigen::MatrixXd out_vertices;
  Eigen::MatrixXi out_faces;
  Eigen::VectorXi out_j;

  igl::copyleft::cgal::wire_mesh(eigen_vertices, eigen_edges, radius, nsides, out_vertices, out_faces, out_j);

  return eigenToTrimesh(out_vertices, out_faces);
}

/* ------------------------------------------------------------------------- */

}
}

