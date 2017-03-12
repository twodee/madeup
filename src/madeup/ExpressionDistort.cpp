#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionCall.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDistort.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionDistort::ExpressionDistort() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionDistort::evaluate(Environment &env) const {
  Co<ExpressionClosure> mesh_closure = env["mesh"];
  if (mesh_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function distort to be given a value named mesh, but no mesh was given.");
  }

  Co<Expression> mesh_value = mesh_closure->evaluate(env);
  ExpressionMesh *mesh_pointer = dynamic_cast<ExpressionMesh *>(mesh_value.GetPointer());
  if (!mesh_pointer) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function distort to be given a mesh, but that's not what you gave it.");
  }

  Co<ExpressionClosure> f_closure = env["f"];
  if (f_closure.IsNull()) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function distort to be given a value named f, but no f was given.");
  }

  Co<Trimesh> mesh = mesh_pointer->toMesh();
  Co<Trimesh> distorted_mesh(new Trimesh(*mesh));

  float *position = mesh->GetPositions();
  float *distorted_position = distorted_mesh->GetPositions();
  for (int i = 0; i < mesh->GetVertexCount(); ++i, position += 3, distorted_position += 3) {
    Co<ExpressionCall> call(new ExpressionCall("f"));
    call->addParameter(Co<Expression>(new ExpressionReal(position[0])));
    call->addParameter(Co<Expression>(new ExpressionReal(position[1])));
    call->addParameter(Co<Expression>(new ExpressionReal(position[2])));
    Co<Expression> xyz = call->evaluate(env);
    ExpressionArrayReference *array_reference = dynamic_cast<ExpressionArrayReference *>(xyz.GetPointer());
    if (array_reference) {
      Co<ExpressionArray> array = array_reference->getArray();
      distorted_position[0] = dynamic_cast<ExpressionReal *>((*array)[0].GetPointer())->toReal();
      distorted_position[1] = dynamic_cast<ExpressionReal *>((*array)[1].GetPointer())->toReal();
      distorted_position[2] = dynamic_cast<ExpressionReal *>((*array)[2].GetPointer())->toReal();
    }
  }

  return Co<Expression>(new ExpressionMesh(distorted_mesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionDistort::write(ostream &out) const {
  out << "(distort f mesh)";
}

/* ------------------------------------------------------------------------- */

}
