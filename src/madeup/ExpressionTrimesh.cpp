#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionTrimesh.h"
#include "madeup/ExpressionUtilities.h"

using namespace td;
using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionTrimesh::ExpressionTrimesh() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionTrimesh::evaluate(Environment &env) const {
  Co<ExpressionArrayReference> positions_array = ExpressionUtilities::lookup<ExpressionArrayReference>("vertices", "array", "trimesh", env);
  Co<ExpressionArrayReference> faces_array = ExpressionUtilities::lookup<ExpressionArrayReference>("faces", "array", "trimesh", env);

  int nvertices = positions_array->getArray()->getSize();
  int nfaces = faces_array->getArray()->getSize();

  vector<QVector3<float>> positions;
  for (int i = 0; i < nvertices; ++i) {
    Co<Expression> element = positions_array->getArray()->operator[](i);
    ExpressionArrayReference *position_array = dynamic_cast<ExpressionArrayReference *>(element.GetPointer());
    if (!position_array || position_array->getArray()->getSize() != 3) {
      std::stringstream ss;
      ss << getSourceLocation().toAnchor() << "I expect function trimesh be given an array of 3-arrays for its vertices parameter. But positions[" << i << "] is not a 3-array.";
      throw MessagedException(ss.str());
    }

    QVector3<float> position;
    for (int d = 0; d < 3; ++d) {
      Co<Expression> component = position_array->getArray()->operator[](d);
      ExpressionNumber *value = dynamic_cast<ExpressionNumber *>(component.GetPointer());
      if (!value) {
        std::stringstream ss;
        ss << getSourceLocation().toAnchor() << "I expect each element of function trimesh's vertices parameter to be an array of 3 numbers. But positions[" << i << "] is not an array of 3 numbers.";
        throw MessagedException(ss.str());
      }
      position[d] = value->toReal();
    }
    positions.push_back(position);
  }

  vector<QVector3<int>> faces;
  for (int i = 0; i < nfaces; ++i) {
    Co<Expression> element = faces_array->getArray()->operator[](i);
    ExpressionArrayReference *face_array = dynamic_cast<ExpressionArrayReference *>(element.GetPointer());
    if (!face_array || face_array->getArray()->getSize() != 3) {
      std::stringstream ss;
      ss << getSourceLocation().toAnchor() << "I expect function trimesh be given an array of 3-arrays for its faces parameter. But faces[" << i << "] is not a 3-array.";
      throw MessagedException(ss.str());
    }

    QVector3<int> face;
    for (int d = 0; d < 3; ++d) {
      Co<Expression> component = face_array->getArray()->operator[](d);
      ExpressionInteger *value = dynamic_cast<ExpressionInteger *>(component.GetPointer());
      if (!value) {
        std::stringstream ss;
        ss << getSourceLocation().toAnchor() << "I expect each element of function trimesh's faces parameter to be an array of 3 integers. But faces[" << i << "] is not an array of 3 integers.";
        throw MessagedException(ss.str());
      }

      face[d] = value->toInteger();

      if (face[d] < 0 || face[d] >= nvertices) {
        std::stringstream ss;
        ss << getSourceLocation().toAnchor() << "I expect each element of function trimesh's faces parameter to be an array of 3 indices into the vertices parameter. But faces[" << i << "] contained index " << value->toInteger() << ", which is not a legal index.";
        throw MessagedException(ss.str());
      }
    }
    faces.push_back(face);
  }

  Co<Trimesh> mesh = Co<Trimesh>(new Trimesh(positions, faces)); 

  QVector3<float> rgb(1.0f, 0.0f, 0.0f);
  Co<Expression> rgb_expression = env[".rgb"]->evaluate(env);
  ExpressionArrayReference *rgb_value = dynamic_cast<ExpressionArrayReference *>(rgb_expression.GetPointer());
  if (rgb_value) {
    Co<ExpressionArray> array = rgb_value->getArray();
    for (int i = 0; i < 3; ++i) {
      Co<Expression> element_value = (*array)[i];
      ExpressionNumber *element_number = dynamic_cast<ExpressionNumber *>(element_value.GetPointer());
      rgb[i] = element_number->toReal();
    }
  }
  
  float *colors = mesh->AllocateVertexColors();
  float *color = colors;
  for (int i = 0; i < nvertices; ++i, color += 3) {
    color[0] = rgb[0];
    color[1] = rgb[1];
    color[2] = rgb[2];
  }

  return Co<Expression>(new ExpressionMesh(mesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionTrimesh::write(ostream &out) const {
  out << "(trimesh)";
}

/* ------------------------------------------------------------------------- */

}
