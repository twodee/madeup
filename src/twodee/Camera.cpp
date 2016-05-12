#include <iostream>

#include "twodee/Camera.h"
#include "twodee/QMath.h"

namespace td {

/* ------------------------------------------------------------------------- */

Camera::Camera() :
  perspective_projection(),
  orthographic_projection(),
  from(0.0f),
  to(0.0f, 0.0f, 1.0f),
  up(0.0f, 1.0f, 0.0f),
  tilt(0.0f),
  aspect_ratio(1.0f) {
  LookAt(from, to, up);  
  SetPerspective(45.0f, 1.0f, 0.01f, 100.0f);
}

/* ------------------------------------------------------------------------- */

Camera::Camera(const QVector3<float>& from,
               const QVector3<float>& to,
               const QVector3<float>& up) :
  perspective_projection(),
  orthographic_projection(),
  tilt(0.0f),
  aspect_ratio(1.0f) {
  LookAt(from, to, up);  
}

/* ------------------------------------------------------------------------- */

void Camera::LookAt(const QVector3<float>& from,
                    const QVector3<float>& to,
                    const QVector3<float>& up) {
  this->from = from;
  this->to = to;

  focal_direction = to - from;
  focal_direction.Normalize();

  right = focal_direction.Cross(up);
  right.Normalize();

  QVector3<float> real_up;
  real_up = right.Cross(focal_direction);
  real_up.Normalize();

  QMatrix4<float> rot;
  rot(0, 0) = right[0];
  rot(0, 1) = right[1];
  rot(0, 2) = right[2];
  rot(0, 3) = 0;

  rot(1, 0) = real_up[0];
  rot(1, 1) = real_up[1];
  rot(1, 2) = real_up[2];
  rot(1, 3) = 0;

  rot(2, 0) = -focal_direction[0];
  rot(2, 1) = -focal_direction[1];
  rot(2, 2) = -focal_direction[2];
  rot(2, 3) = 0;

  rot(3, 0) = 0;
  rot(3, 1) = 0;
  rot(3, 2) = 0;
  rot(3, 3) = 1;

  this->up = real_up;
  view = QMatrix4<float>::GetRotate(tilt, QVector3<float>(1.0f, 0.0f, 0.0f)) * rot * QMatrix4<float>::GetTranslate(-from[0], -from[1], -from[2]);
}

/* ------------------------------------------------------------------------- */

void Camera::SetOrthographic(float l, float r, float b, float t, float n, float f) {
  orthographic_projection = QMatrix4<float>::GetOrthographic(l, r, b, t, n, f);
  orthographic_frustum[0] = l;
  orthographic_frustum[1] = r;
  orthographic_frustum[2] = b;
  orthographic_frustum[3] = t;
  orthographic_frustum[4] = n;
  orthographic_frustum[5] = f;
  is_perspective = false;
}

/* ------------------------------------------------------------------------- */

void Camera::SetOrthographic(float min_width, float min_height, float aspect, float n, float f) {
  // Taller than we are wide. Y expands.
  if (aspect < 1.0f) {
    SetOrthographic(-min_width * 0.5f, min_width * 0.5f, -min_height * 0.5f / aspect, min_height * 0.5f / aspect, n, f);
  } else {
    SetOrthographic(-min_width * 0.5f * aspect, min_width * 0.5f * aspect, -min_height * 0.5f, min_height * 0.5f, n, f);
  }
  aspect_ratio = aspect;
}

/* ------------------------------------------------------------------------- */

void Camera::SetPerspective(float l, float r, float b, float t, float n, float f) {
  perspective_projection = QMatrix4<float>::GetPerspective(l, r, b, t, n, f);
  perspective_frustum[0] = l;
  perspective_frustum[1] = r;
  perspective_frustum[2] = b;
  perspective_frustum[3] = t;
  perspective_frustum[4] = n;
  perspective_frustum[5] = f;
  is_perspective = true;
}

/* ------------------------------------------------------------------------- */

void Camera::SetPerspective(float fov_y, float aspect_ratio, float near, float far) {
  float y = near * tan(fov_y * PI / 180.0f / 2.0f);
  float x = y * aspect_ratio;
  this->aspect_ratio = aspect_ratio;
  SetPerspective(-x, x, -y, y, near, far);
  this->fov_y = fov_y;
}

/* ------------------------------------------------------------------------- */

void Camera::Move(float delta_x, float delta_y, float delta_z) {
  QVector3<float> delta(delta_x, delta_y, delta_z);
  from += delta;
  to += delta;
  LookAt(from, to, up);
}

/* ------------------------------------------------------------------------- */

void Camera::Roll(float degrees) {
  QMatrix4<float> r = QMatrix4<float>::GetRotate(degrees, focal_direction);
  LookAt(from, to, r * up);
}

/* ------------------------------------------------------------------------- */

void Camera::Pitch(float degrees) {
  QMatrix4<float> r = QMatrix4<float>::GetRotate(degrees, right);
  to = from;
  to += r * focal_direction;
  up = r * up;
  LookAt(from, to, up);
}

/* ------------------------------------------------------------------------- */

void Camera::Yaw(float degrees) {
  QMatrix4<float> r = QMatrix4<float>::GetRotate(degrees, up);
  to = from;
  to += r * focal_direction;
  LookAt(from, to, up);
}

/* ------------------------------------------------------------------------- */

void Camera::Advance(float delta) {
  QVector3<float> offset = focal_direction;
  offset *= delta;
  from += offset;
  to += offset;
  LookAt(from, to, up);
}

/* ------------------------------------------------------------------------- */

void Camera::Strafe(float delta) {
  QVector3<float> offset = right;
  offset *= delta;
  from += offset;
  to += offset;
  LookAt(from, to, up);
}

/* ------------------------------------------------------------------------- */

const QVector3<float>& Camera::GetFrom() const {
  return from;
}

/* ------------------------------------------------------------------------- */

const QVector3<float>& Camera::GetTo() const {
  return to;
}

/* ------------------------------------------------------------------------- */

void Camera::ElevateTo(float height) {
  from[1] = height;  
  to[1] = height;
  LookAt(from, to, up);
}

/* ------------------------------------------------------------------------- */

#if 0
void Camera::SetHeight(float delta) {
  QVector3<float> offset(up);
  offset *= delta;

  from += offset;  
  to += offset;
  LookAt(from, to, up);
}
#endif

/* ------------------------------------------------------------------------- */

void Camera::SetTilt(float offset) {
  tilt += offset;
  const float MAX_TILT = 30.0f;
  if (tilt > MAX_TILT) {
    tilt = MAX_TILT;
  } else if (tilt < -MAX_TILT) {
    tilt = -MAX_TILT;
  }
  LookAt(from, to, up);
}

/* ------------------------------------------------------------------------- */

void Camera::SwitchToOrthographic() {
  is_perspective = false;
}

/* ------------------------------------------------------------------------- */

void Camera::SwitchToPerspective() {
  is_perspective = true;
}

/* ------------------------------------------------------------------------- */

void Camera::SetFieldOfViewY(float fov_y) {
  SetPerspective(fov_y, aspect_ratio, perspective_frustum[4], perspective_frustum[5]);
}

/* ------------------------------------------------------------------------- */

float Camera::GetFieldOfViewY() const {
  return fov_y;
}

/* ------------------------------------------------------------------------- */

float Camera::GetPerspectiveNear() const {
  return perspective_frustum[4];
}

/* ------------------------------------------------------------------------- */

float Camera::GetPerspectiveFar() const {
  return perspective_frustum[5];
}

/* ------------------------------------------------------------------------- */

float Camera::GetOrthographicNear() const {
  return orthographic_frustum[4];
}

/* ------------------------------------------------------------------------- */

float Camera::GetOrthographicFar() const {
  return orthographic_frustum[5];
}

/* ------------------------------------------------------------------------- */

void Camera::SetPerspectiveNear(float near) {
  SetPerspective(fov_y, aspect_ratio, near, perspective_frustum[5]);
}

/* ------------------------------------------------------------------------- */

void Camera::SetPerspectiveFar(float far) {
  SetPerspective(fov_y, aspect_ratio, perspective_frustum[4], far);
}

/* ------------------------------------------------------------------------- */

void Camera::SetOrthographicNear(float near) {
  SetOrthographic(orthographic_frustum[0], orthographic_frustum[1], orthographic_frustum[2], orthographic_frustum[3], near, orthographic_frustum[5]);
}

/* ------------------------------------------------------------------------- */

void Camera::SetOrthographicFar(float far) {
  SetOrthographic(orthographic_frustum[0], orthographic_frustum[1], orthographic_frustum[2], orthographic_frustum[3], orthographic_frustum[4], far);
}

/* ------------------------------------------------------------------------- */

void Camera::SetOrthographicLeft(float l) {
  SetOrthographic(l, orthographic_frustum[1], orthographic_frustum[2], orthographic_frustum[3], orthographic_frustum[4], orthographic_frustum[5]);
}

/* ------------------------------------------------------------------------- */

void Camera::SetOrthographicRight(float r) {
  SetOrthographic(orthographic_frustum[0], r, orthographic_frustum[2], orthographic_frustum[3], orthographic_frustum[4], orthographic_frustum[5]);
}

/* ------------------------------------------------------------------------- */

void Camera::SetOrthographicBottom(float b) {
  SetOrthographic(orthographic_frustum[0], orthographic_frustum[1], b, orthographic_frustum[3], orthographic_frustum[4], orthographic_frustum[5]);
}

/* ------------------------------------------------------------------------- */

void Camera::SetOrthographicTop(float t) {
  SetOrthographic(orthographic_frustum[0], orthographic_frustum[1], orthographic_frustum[2], t, orthographic_frustum[4], orthographic_frustum[5]);
}

/* ------------------------------------------------------------------------- */

const QVector3<float>& Camera::GetUp() const {
  return up;
}

/* ------------------------------------------------------------------------- */

QVector3<float> Camera::GetForward() const {
  return QVector3<float>(-view(2, 0), -view(2, 1), -view(2, 2)); 
}

/* ------------------------------------------------------------------------- */

QVector3<float> Camera::GetRight() const {
  return QVector3<float>(view(0, 0), view(0, 1), view(0, 2)); 
}

/* ------------------------------------------------------------------------- */

QVector3<float> Camera::GetRealUp() const {
  return QVector3<float>(view(1, 0), view(1, 1), view(1, 2)); 
}

/* ------------------------------------------------------------------------- */

}
