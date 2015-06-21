#ifndef QMATH_H
#define QMATH_H

#include "QVector.h"
#include "QMatrix.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 Gets the product of the specified matrices.
 @param a First operand
 @param b Second operand
 */
template<class T, int nrows, int ncols_a, int ncols_b>
QMatrix<T, nrows, ncols_b> operator*(const QMatrix<T, nrows, ncols_a>& a,
                                     const QMatrix<T, ncols_a, ncols_b>& b) {
  QMatrix<T, nrows, ncols_b> m;

  // Take each row...
  for (int i = 0; i < nrows; i++) {
    // ... with each column ...
    for (int j = 0; j < ncols_b; j++) {
      // ... and dot product them to get new entry.
      m(i, j) = 0;
      for (int k = 0; k < ncols_a; k++) {
        m(i, j) += a(i, k) * b(k, j);
      }
    }
  }

  return m;
} 

/* ------------------------------------------------------------------------- */

/**
 Gets the product of the specified matrix and vector.
 @param m Matrix operand
 @param v Vector operand
 */
template <class T, int nrows, int ncols>
QVector<T, nrows> operator*(const QMatrix<T, nrows, ncols> &m,
                            const QVector<T, ncols> &v) {
  QVector<T, nrows> ans((T) 0);

  for (int r = 0; r < nrows; ++r) {
    for (int c = 0; c < ncols; ++c) {
      ans[r] += m(r, c) * v[c];
    }
  }

  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the product of the specified matrix and vector, where the vector has
 one less dimension than the matrix and the bottom row of the matrix is
 the vector [0 0 ... 1]. The missing component of the vector is assumed 1.
 This method can be used to transform points by a transformation matrix
 in homogeneous space.
 @param m Matrix operand
 @param v Vector operand
 */
template <class T, int nrows, int ncols>
QVector<T, nrows - 1> operator*(const QMatrix<T, nrows, ncols> &m,
                                const QVector<T, ncols - 1> &v) {
  QVector<T, nrows - 1> ans((T) 0);

  for (int r = 0; r < nrows - 1; ++r) {
    for (int c = 0; c < ncols - 1; ++c) {
      ans[r] += m(r, c) * v[c];
    }
    ans[r] += m(r, ncols - 1);
  }

  return ans;
}

/* ------------------------------------------------------------------------- */

#if 0
template<class T, int ndims>
QVector<T, ndims>& operator+(const QVector<T, ndims>& a,
                             const QVector<T, ndims>& b) {
  QVector<T, ndims> ans = a;
  ans += b;
  return ans;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims>& operator+(const QVector<T, ndims>& v, T scalar) {
  QVector<T, ndims> ans = v;
  ans += v;
  return ans;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims>& operator/(const QVector<T, ndims>& v, T scalar) {
  QVector<T, ndims> ans = v;
  ans /= v;
  return ans;
}
#endif

/* ------------------------------------------------------------------------- */

}

#endif
