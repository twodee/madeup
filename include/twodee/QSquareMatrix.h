#ifndef QSQUAREMATRIX_H
#define QSQUAREMATRIX_H

#include "QMatrix.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A square matrix.
 */
template<class T, int nrows> class QSquareMatrix : public QMatrix<T, nrows, nrows> {
  public:
    /**
     Creates a new matrix. The initial values are unspecified.
     */
    QSquareMatrix();

    /**
     Creates a new matrix, all of whose diagonal elements are the specified
     value and whose off-diagonal elements are 0.
     @param value Initial value of each diagonal element
     */
    QSquareMatrix(T value);

    /**
     Creates a new matrix, using the specified source for its initial values.
     Assumes data contains enough values to fill the matrix and that data is
     column-major order (i.e., m(0, 0), m(1, 0), m(2, 0), ...).
     @param src Initial element source
     */
    QSquareMatrix(const T *src);

    /**
     Creates a new matrix, using the supertype matrix as a source.
     @param src The NxN matrix to be copied.
     */
    explicit QSquareMatrix(const QMatrix<T, nrows, nrows>& src);

    /**
     Get a matrix that translates by the specified offset vector.
     @param offset Offset vector
     */
    static QSquareMatrix<T, nrows> GetTranslate(const QVector<T, nrows - 1>& offset);

    /**
     Get a matrix that scales by the specified factor vector.
     @param scale Scale vector
     */
    static QSquareMatrix<T, nrows> GetScale(const QVector<T, nrows - 1>& scale);
};

/* ------------------------------------------------------------------------- */

template<class T, int nrows>
QSquareMatrix<T, nrows>::QSquareMatrix()
  : QMatrix<T, nrows, nrows>() {
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows>
QSquareMatrix<T, nrows>::QSquareMatrix(T value)
  : QMatrix<T, nrows, nrows>((T) 0) {
  for (int i = 0; i < nrows; ++i) {
    (*this)(i, i) = value;
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows>
QSquareMatrix<T, nrows>::QSquareMatrix(const T *src)
  : QMatrix<T, nrows, nrows>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows>
QSquareMatrix<T, nrows>::QSquareMatrix(const QMatrix<T, nrows, nrows>& src)
  : QMatrix<T, nrows, nrows>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows>
QSquareMatrix<T, nrows> QSquareMatrix<T, nrows>::GetTranslate(const QVector<T, nrows - 1>& offset) {
  QSquareMatrix<T, nrows> m(1);

  for (int i = 0; i < nrows - 1; ++i) {
    m(i, nrows - 1) = offset[i];
  }

  return m;
}

/* ------------------------------------------------------------------------- */

template<class T, int nrows>
QSquareMatrix<T, nrows> QSquareMatrix<T, nrows>::GetScale(const QVector<T, nrows - 1>& scale) {
  QSquareMatrix<T, nrows> m(1);

  for (int i = 0; i < nrows - 1; ++i) {
    m(i, i) = scale[i];
  }

  return m;
}

/* ------------------------------------------------------------------------- */

}

#endif
