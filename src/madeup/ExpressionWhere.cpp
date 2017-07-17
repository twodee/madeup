#include <iostream>

#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionCamera.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionWhere.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionWhere::ExpressionWhere() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionWhere::evaluate(Environment &env) const {
  QVector3<float> position = env.getTurtle().position;
  Co<ExpressionArray> new_array(new ExpressionArray(3));
  for (int i = 0; i < 3; ++i) {
    new_array->setElement(i, Co<Expression>(new ExpressionReal(position[i])));
  }
  return Co<Expression>(new ExpressionArrayReference(new_array));
}

/* ------------------------------------------------------------------------- */

void ExpressionWhere::write(ostream &out) const {
  out << "(where)";
}

/* ------------------------------------------------------------------------- */

ExpressionForward::ExpressionForward() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionForward::evaluate(Environment &env) const {
  QVector3<float> axis = env.getTurtle().camera.GetForward();
  Co<ExpressionArray> new_array(new ExpressionArray(3));
  for (int i = 0; i < 3; ++i) {
    new_array->setElement(i, Co<Expression>(new ExpressionReal(axis[i])));
  }
  return Co<Expression>(new ExpressionArrayReference(new_array));
}

/* ------------------------------------------------------------------------- */

void ExpressionForward::write(ostream &out) const {
  out << "(forward)";
}

/* ------------------------------------------------------------------------- */

ExpressionRight::ExpressionRight() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionRight::evaluate(Environment &env) const {
  QVector3<float> axis = env.getTurtle().camera.GetRight();
  Co<ExpressionArray> new_array(new ExpressionArray(3));
  for (int i = 0; i < 3; ++i) {
    new_array->setElement(i, Co<Expression>(new ExpressionReal(axis[i])));
  }
  return Co<Expression>(new ExpressionArrayReference(new_array));
}

/* ------------------------------------------------------------------------- */

void ExpressionRight::write(ostream &out) const {
  out << "(right)";
}

/* ------------------------------------------------------------------------- */

ExpressionUp::ExpressionUp() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionUp::evaluate(Environment &env) const {
  QVector3<float> axis = env.getTurtle().camera.GetRealUp();
  Co<ExpressionArray> new_array(new ExpressionArray(3));
  for (int i = 0; i < 3; ++i) {
    new_array->setElement(i, Co<Expression>(new ExpressionReal(axis[i])));
  }
  return Co<Expression>(new ExpressionArrayReference(new_array));
}

/* ------------------------------------------------------------------------- */

void ExpressionUp::write(ostream &out) const {
  out << "(up)";
}

/* ------------------------------------------------------------------------- */

ExpressionView::ExpressionView() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionView::evaluate(Environment &env) const {
  td::Camera camera = env.getTurtle().camera;
  return Co<Expression>(new ExpressionCamera(camera));
}

/* ------------------------------------------------------------------------- */

void ExpressionView::write(ostream &out) const {
  out << "(view)";
}

/* ------------------------------------------------------------------------- */

}
