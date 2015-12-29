#include <iostream>

#define IGL_NO_CORK
#include <igl/copyleft/boolean/mesh_boolean.h>
#include <igl/per_corner_normals.h>

#include <madeup/ExpressionArray.h>
#include <madeup/ExpressionClosure.h>
#include <madeup/ExpressionNumber.h>
#include <madeup/MeshBoolean.h>

using namespace td;

namespace madeup {
namespace MeshBoolean {

/* ------------------------------------------------------------------------- */

void TrimeshToEigen(const Trimesh &mesh,
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

Trimesh *EigenToTrimesh(const Eigen::MatrixXd &vertices,
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

Trimesh *construct(const Trimesh &a,
                   const Trimesh &b,
                   operation_t operation) {

  igl::copyleft::boolean::MeshBooleanType igl_operation;
  switch (operation) {
    case UNION:
      igl_operation = igl::copyleft::boolean::MESH_BOOLEAN_TYPE_UNION;
      break;
    case DIFFERENCE:
      igl_operation = igl::copyleft::boolean::MESH_BOOLEAN_TYPE_MINUS;
      break;
    case INTERSECTION:
      igl_operation = igl::copyleft::boolean::MESH_BOOLEAN_TYPE_INTERSECT;
      break;
  }

  Eigen::MatrixXd vertices[3];
  Eigen::MatrixXi faces[3];

  TrimeshToEigen(a, vertices[0], faces[0]);
  TrimeshToEigen(b, vertices[1], faces[1]);

  Eigen::VectorXi J;
  igl::copyleft::boolean::mesh_boolean(vertices[0], faces[0], vertices[1], faces[1], igl_operation, vertices[2], faces[2], J);

  Eigen::MatrixXd N_corners;
  igl::per_corner_normals(vertices[2], faces[2], 20, N_corners);

  std::cout << "vertices[2].rows(): " << vertices[2].rows() << std::endl;
  std::cout << "faces[2].rows(): " << faces[2].rows() << std::endl;
  return EigenToTrimesh(vertices[2], faces[2]);
}

/* ------------------------------------------------------------------------- */

Co<Expression> construct_and_color(ExpressionMesh *l_mesh,
                                   ExpressionMesh *r_mesh,
                                   Environment &env,
                                   operation_t operation) {
  Co<Trimesh> l = l_mesh->toMesh();
  Co<Trimesh> r = r_mesh->toMesh();
  Co<Trimesh> diff = MeshBoolean::construct(*l, *r, operation);

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

  return Co<Expression>(new ExpressionMesh(diff));
}

/* ------------------------------------------------------------------------ */

}
}

