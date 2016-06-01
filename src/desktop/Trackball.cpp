#include "twodee/QMath.h"
#include "Trackball.h"

namespace td {

/* ------------------------------------------------------------------------- */

Trackball::Trackball() :
  to_rotation(1.0f),
  mouse_at(0),
  mouse_delta(0),
  last_rotation(1.0) {
}

/* ------------------------------------------------------------------------- */

void Trackball::Reset() {
  to_rotation = QMatrix4<float>(1.0f);
}

/* ------------------------------------------------------------------------- */

void Trackball::Rotate(float angle, const QVector3<float>& axis) {
  to_rotation = QMatrix4<float>::GetRotate(angle, axis) * to_rotation;
}

/* ------------------------------------------------------------------------- */

void Trackball::Autorotate() {
  to_rotation = last_rotation * to_rotation;
}

/* ------------------------------------------------------------------------- */

const QMatrix4<float>& Trackball::GetMatrix() const {
  return to_rotation;
}

/* ------------------------------------------------------------------------- */

void Trackball::Start(int x, int y) {
  mouse_at[0] = x;
  mouse_at[1] = y;
  mouse_delta[0] = mouse_delta[1] = 0;
}

/* ------------------------------------------------------------------------- */

void Trackball::Drag(int x, int y, float factor) {
  mouse_delta[0] = x - mouse_at[0];
  mouse_delta[1] = y - mouse_at[1];

  if (mouse_at[0] != x || mouse_at[1] != y) {
    // Get vector for previous stop
    float min_span = QVector2<float>(width, height).GetLength() * 0.5f;
    QVector3<float> prev((width * 0.5f - mouse_at[0]) / min_span,
                         (height * 0.5f - mouse_at[1]) / min_span, 0.0f);
    prev[2] = 1.0 - prev[0] * prev[0] - prev[1] * prev[1];
    if (prev[2] < 0.0f) {
      prev[2] = 0.0f;
      prev.Normalize();
    } else {
      prev[2] = sqrtf(prev[2]);
    }

    // Get vector for current stop
    mouse_at[0] = x;
    mouse_at[1] = y;
    QVector3<float> curr((width * 0.5f - mouse_at[0]) / min_span,
                         (height * 0.5f - mouse_at[1]) / min_span, 0.0f);
    curr[2] = 1.0 - curr[0] * curr[0] - curr[1] * curr[1];
    if (curr[2] < 0.0f) {
      curr[2] = 0.0f;
      curr.Normalize();
    } else {
      curr[2] = sqrtf(curr[2]);
    }

    // What's the angle between these two vectors?
    float dot = prev.Dot(curr);
    if (dot > 1.0f) {
      dot = 1.0f;
    } else if (dot < -1.0f) {
      dot = -1.0f;
    }

    float angle = acos(dot) * 180.0f / PI * factor;
    QVector3<float> axis = curr.Cross(prev);
    axis.Normalize();

    // TODO
    if (axis[2] == 1.0f) {
      axis[2] = -1.0f;
    } else if (axis[2] == -1.0f) {
      axis[2] = 1.0f;
    }

    last_rotation = QMatrix4<float>::GetRotate(angle, axis);
    to_rotation = last_rotation * to_rotation;
  }
}

/* ------------------------------------------------------------------------- */

bool Trackball::Stop() {
  return abs(mouse_delta[0]) > 3 || abs(mouse_delta[1]) > 3;
}

/* ------------------------------------------------------------------------- */

void Trackball::SetViewport(int width, int height, int x, int y) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;  
}

/* ------------------------------------------------------------------------- */

}
