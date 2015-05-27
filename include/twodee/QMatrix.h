#ifndef QMATRIX_H
#define QMATRIX_H

#include <cassert>
#include <iostream>

#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A matrix.
 */
template<class T, int nrows, int ncols> class QMatrix {
  public:
    /**
     Creates a new matrix. The initial values are unspecified.
     */
    QMatrix();

    /**
     Creates a new matrix, all of whose values are the specified value.
     @param value Initial value of each element
     */
    explicit QMatrix(T value);

    /**
     Creates a new matrix, using the specified source for its initial values.
     Assumes data contains enough values to fill the matrix and that data is
     column-major order (i.e., m(0, 0), m(1, 0), m(2, 0), ...).
     @param src Initial element source
     */
    explicit QMatrix(const T *src);

    /**
     Gets the element at the specified row and column.
     @param r Row
     @param c Column
     */
    T& operator()(int r, int c);

    /**
     Gets the element at the specified row and column.
     @param r Row
     @param c Column
     */
    const T& operator()(int r, int c) const;

  protected:
    /** The elements */
    T data[nrows * ncols];
};

/* ------------------------------------------------------------------------- */

#if 0
template<class T, int nrows, int ncols_a, int ncols_b>
td::QMatrix<T, nrows, ncols_b> operator*(const td::QMatrix<T, nrows, ncols_a>& a, const td::QMatrix<T, ncols_a, ncols_b>& b) {
  QMatrix<T, nrows, ncols_b> result;

  // Take each row...
  for (int i = 0; i < nrows; i++) {
    // ... with each column ...
    for (int j = 0; j < ncols_b; j++) {
      // ... and dot product them to get new entry. Or least a 0.
      result(i, j) = 0;
      for (int k = 0; k < ncols_a; k++) {
        result(i, j) += a(i, k) * b(k, j);
      }
    }
  }

  return result;
}
#endif

/* ------------------------------------------------------------------------- */

template<class T, int nrows, int ncols>
std::ostream& operator<<(std::ostream& stream,
                         const QMatrix<T, nrows, ncols>& m) {
  for (int r = 0; r < nrows; ++r) {
    stream << std::endl << m(r, 0);
    for (int c = 1; c < ncols; ++c) {
      stream << ' ' << m(r, c);
    }
  }
  return stream;
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows, int ncols>
std::istream& operator>>(std::istream& stream,
                         QMatrix<T, nrows, ncols>& m) {
  for (int r = 0; r < nrows; ++r) {
    for (int c = 0; c < ncols; ++c) {
      stream >> m(r, c);
    }
  }
  return stream;
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows, int ncols>
bool operator==(const QMatrix<T, nrows, ncols>& a,
                const QMatrix<T, nrows, ncols>& b) {
  for (int r = 0; r < nrows; ++r) {
    for (int c = 0; c < ncols; ++c) {
      if (a(r, c) != b(r, c)) {
        return false;
      }
    }
  }
  return true;
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows, int ncols>
QMatrix<T, nrows, ncols>::QMatrix() {
  assert(nrows > 0);
  assert(ncols > 0);
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows, int ncols>
QMatrix<T, nrows, ncols>::QMatrix(const T *src) {
  assert(nrows > 0);
  assert(ncols > 0);

  memcpy(this->data, src, sizeof(T) * nrows * ncols);
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows, int ncols>
QMatrix<T, nrows, ncols>::QMatrix(T val) {
  assert(nrows > 0);
  assert(ncols > 0);

  for (int i = 0; i < nrows * ncols; ++i) {
    data[i] = val;
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows, int ncols>
inline T& QMatrix<T, nrows, ncols>::operator()(int r,
                                               int c) {
  assert(r >= 0 && r < nrows);
  assert(c >= 0 && c < ncols);
  return data[r + c * nrows];
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows, int ncols>
inline const T& QMatrix<T, nrows, ncols>::operator()(int r,
                                                     int c) const {
  assert(r >= 0 && r < nrows);
  assert(c >= 0 && c < ncols);
  return data[r + c * nrows];
}

/* ------------------------------------------------------------------------- */

}

#endif
