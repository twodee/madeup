#ifndef QMATRIX4_H
#define QMATRIX4_H

#include "QVector3.h"
#include "QSquareMatrix.h"
#include "Utilities.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A 4x4 matrix.
 */
template<class T> class QMatrix4 : public QSquareMatrix<T, 4> {
  public:
    /**
     Creates a new matrix. The initial values are unspecified.
     */
    QMatrix4();

    /**
     Creates a new matrix, all of whose diagonal elements are the specified
     value and whose off-diagonal elements are 0.
     @param value Initial value of each diagonal element
     */
    QMatrix4(T value);

    /**
     Creates a new matrix, using the specified source for its initial values.
     Assumes data contains enough values to fill the matrix and that data is
     column-major order (i.e., m(0, 0), m(1, 0), m(2, 0), ...).
     @param src Initial element source
     */
    QMatrix4(const T *src);

    /**
     Creates a new matrix, using the supertype matrix as a source.
     @param src The 4x4 matrix to be copied.
     */
    QMatrix4(const QSquareMatrix<T, 4>& src);

    /**
     Creates a new matrix, using the supertype matrix as a source.
     @param src The 4x4 matrix to be copied.
     */
    QMatrix4(const QMatrix<T, 4, 4>& src);

    QMatrix4<T> GetOrthonormalInverse() const;

    /**
     Gets a matrix that translates by the specified offsets.
     @param x Offset on x-axis
     @param y Offset on y-axis
     @param z Offset on z-axis
     */
    static QMatrix4<T> GetTranslate(T x, T y, T z);

    /**
     Gets a matrix that scales by the specified factors.
     @param x Scale factor on x-axis
     @param y Scale factor on y-axis
     @param z Scale factor on z-axis
     */
    static QMatrix4<T> GetScale(T x, T y, T z);

    /**
     Gets a matrix that rotates the specified number of degrees around the
     specified axis. The axis is assumed to be normalized.
     @param degrees Degrees to rotate
     @param axis Axis around which to rotate
     */
    static QMatrix4<T> GetRotate(T degrees, const QVector3<T>& axis);

    static QMatrix4<T> GetPlanarProjection(const QVector3<T> &from, const QVector3<T> &plane_normal, const QVector3<T> &plane_point);

    /**
     Gets a matrix that projects the specified volume orthographically into
     x [-1, 1], y [-1 1], and z [-1 1].
     @param left x-value that maps to -1.
     @param right x-value that maps to 1.
     @param bottom y-value that maps to -1.
     @param top y-value that maps to 1.
     @param near z-value that maps to -1.
     @param far z-value that maps to 1.
     */
    static QMatrix4<T> GetOrthographic(T left,
                                       T right,
                                       T bottom,
                                       T top,
                                       T near = -1,
                                       T far = 1);

    /**
     Gets a matrix that unprojects the volume x [-1, 1], y [-1 1], and z [-1 1]
     into the specified eye-space volume.
     @param left x-value that maps to -1.
     @param right x-value that maps to 1.
     @param bottom y-value that maps to -1.
     @param top y-value that maps to 1.
     @param near z-value that maps to -1.
     @param far z-value that maps to 1.
     */
    static QMatrix4<T> GetOrthographicInverse(T left,
                                              T right,
                                              T bottom,
                                              T top,
                                              T near = -1,
                                              T far = 1);

    /**
     Gets a matrix that projects the specified volume with perspective into
     x [-1, 1], y [-1 1], and z [-1 1].
     @param left Near plane x-value that maps to -1.
     @param right Near plane x-value that maps to 1.
     @param bottom Near plane y-value that maps to -1.
     @param top Near plane y-value that maps to 1.
     @param near z-value that maps to -1.
     @param far z-value that maps to 1.
     */
    static QMatrix4<T> GetPerspective(T left,
                                      T right,
                                      T bottom,
                                      T top,
                                      T near,
                                      T far);

    /**
     Gets a matrix that projects the specified volume with perspective into
     x [-1, 1], y [-1 1], and z [-1 1].
     @param fov_y Field of view, in degrees, along y-axis
     @param aspect Aspect ratio
     @param near z-value that maps to -1.
     @param far z-value that maps to 1.
     */
    static QMatrix4<T> GetPerspective(T fov_y,
                                      T aspect,
                                      T near,
                                      T far);
};

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T>::QMatrix4()
  : QSquareMatrix<T, 4>() {
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T>::QMatrix4(T value)
  : QSquareMatrix<T, 4>((T) 0) {
  for (int i = 0; i < 4; ++i) {
    (*this)(i, i) = value;
  }
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T>::QMatrix4(const T *src)
  : QSquareMatrix<T, 4>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T>::QMatrix4(const QSquareMatrix<T, 4>& src)
  : QSquareMatrix<T, 4>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T>::QMatrix4(const QMatrix<T, 4, 4>& src)
  : QSquareMatrix<T, 4>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetTranslate(T x, T y, T z) {
  return QSquareMatrix<T, 4>::GetTranslate(QVector3<T>(x, y, z));
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetScale(T x, T y, T z) {
  return QSquareMatrix<T, 4>::GetScale(QVector3<T>(x, y, z));
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetRotate(T degrees, const QVector3<T>& axis) {
  T radians = (T) (degrees * PI / 180);
  T sine = (T) sin(radians);
  T cosine = (T) cos(radians);
  T cos_rec = 1 - cosine;

  QMatrix4<T> m(1);

  m(0, 0) = cos_rec * axis[0] * axis[0] + cosine;
  m(0, 1) = cos_rec * axis[0] * axis[1] - sine * axis[2];
  m(0, 2) = cos_rec * axis[0] * axis[2] + sine * axis[1];

  m(1, 0) = cos_rec * axis[1] * axis[0] + sine * axis[2];
  m(1, 1) = cos_rec * axis[1] * axis[1] + cosine;
  m(1, 2) = cos_rec * axis[1] * axis[2] - sine * axis[0];

  m(2, 0) = cos_rec * axis[2] * axis[0] - sine * axis[1];
  m(2, 1) = cos_rec * axis[2] * axis[1] + sine * axis[0];
  m(2, 2) = cos_rec * axis[2] * axis[2] + cosine;

  return m;
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetOrthographic(T left,
                                         T right,
                                         T bottom,
                                         T top,
                                         T near,
                                         T far) {
  QMatrix4<T> m(1);
  
  m(0, 0) = 2 / (right - left);
  m(1, 1) = 2 / (top - bottom);
  m(2, 2) = 2 / (near - far);

  m(0, 3) = -(right + left) / (right - left);
  m(1, 3) = -(top + bottom) / (top - bottom);
  m(2, 3) = (near + far) / (near - far);

  return m;
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetOrthographicInverse(T left,
                                                T right,
                                                T bottom,
                                                T top,
                                                T near,
                                                T far) {
  T width = right - left;
  T height = top - bottom;
  T depth = near - far;
  /* return GetScale(width * 0.5f, height * 0.5f, depth * 0.5f) * */
         /* GetTranslate((right + left) / width, (top + bottom) / height, (far + near) / depth); */

  // TODO is this okay after switch?
  QMatrix4<T> m = GetScale(width * 0.5f, height * 0.5f, depth * 0.5f);
  m(0, 3) = (right + left) * 0.5f;
  m(1, 3) = (top + bottom) * 0.5f;
  m(2, 3) = (far + near) * 0.5f;

  return m;
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetPerspective(T left,
                                        T right,
                                        T bottom,
                                        T top,
                                        T near,
                                        T far) {
  QMatrix4<T> m(1);
  
  m(0, 0) = 2 * near / (right - left);
  m(0, 2) = (right + left) / (right - left);

  m(1, 1) = 2 * near / (top - bottom);
  m(1, 2) = (top + bottom) / (top - bottom);

  m(2, 2) = (near + far) / (near - far);
  m(2, 3) = 2 * far * near / (near - far);

  m(3, 2) = -1.0f;
  m(3, 3) = 0.0f;

  return m;
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetPerspective(T fov_y,
                                        T aspect,
                                        T near,
                                        T far) {
  T y = near * tan(fov_y * PI / 360);
  T x = y * aspect;
  return GetPerspective(-x, x, -y, y, near, far);
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetOrthonormalInverse() const {
  QMatrix4<T> inv;
  inv(0, 0) = (*this)(0, 0);
  inv(0, 1) = (*this)(1, 0);
  inv(0, 2) = (*this)(2, 0);
  inv(0, 3) = -((*this)(0, 3) * (*this)(0, 0) + (*this)(1, 3) * (*this)(1, 0) + (*this)(2, 3) * (*this)(2, 0));
  inv(1, 0) = (*this)(0, 1);
  inv(1, 1) = (*this)(1, 1);
  inv(1, 2) = (*this)(2, 1);
  inv(1, 3) = -((*this)(0, 3) * (*this)(0, 1) + (*this)(1, 3) * (*this)(1, 1) + (*this)(2, 3) * (*this)(2, 1));
  inv(2, 0) = (*this)(0, 2);
  inv(2, 1) = (*this)(1, 2);
  inv(2, 2) = (*this)(2, 2);
  inv(2, 3) = -((*this)(0, 3) * (*this)(0, 2) + (*this)(1, 3) * (*this)(1, 2) + (*this)(2, 3) * (*this)(2, 2));

  inv(3, 0) = inv(3, 1) = inv(3, 2) = 0;
  inv(3, 3) = 1;

  return inv;
}

/* ------------------------------------------------------------------------- */

template<class T>
QMatrix4<T> QMatrix4<T>::GetPlanarProjection(const QVector3<T> &from, const QVector3<T> &plane_normal, const QVector3<T> &plane_point) {
  float a = -from.Dot(plane_normal);
  float c = plane_point.Dot(plane_normal);
  float b = plane_point.Dot(plane_normal) + a;

  QMatrix4<T> m;

  m(0, 0) = from[0] * plane_normal[0] + b;
  m(0, 1) = from[0] * plane_normal[1];
  m(0, 2) = from[0] * plane_normal[2];
  m(0, 3) = -from[0] * c;

  m(1, 0) = from[1] * plane_normal[0];
  m(1, 1) = from[1] * plane_normal[1] + b;
  m(1, 2) = from[1] * plane_normal[2];
  m(1, 3) = -from[1] * c;

  m(2, 0) = from[2] * plane_normal[0];
  m(2, 1) = from[2] * plane_normal[1];
  m(2, 2) = from[2] * plane_normal[2] + b;
  m(2, 3) = -from[2] * c;

  m(3, 0) = plane_normal[0];
  m(3, 1) = plane_normal[1];
  m(3, 2) = plane_normal[2];
  m(3, 3) = a;

  return m;
}

/* ------------------------------------------------------------------------- */

}

#endif
