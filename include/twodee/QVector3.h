#ifndef QVECTOR3_H
#define QVECTOR3_H

#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A three-dimensional vector.
 */
template<class T> class QVector3 : public QVector<T, 3> {
  public:
    /**
     Creates a vector. Its initial values are undefined.
     */
    QVector3();

    /**
     Creates a vector, all of whose elements are the specified value.
     @param val Initial value of all elements.
     */
    explicit QVector3(T val);

    /**
     Creates a vector, all of whose non-terminal elements are the first
     specified value, and whose terminal element is the second specified value.
     This may be useful for vectors in homogeneous space.
     @param first_vals Initial value of all non-terminal elements.
     @param last_val Initial value of all terminal element.
     */
    QVector3(T first_vals, T last_val);

    /**
     Creates a vector whose elements are retrieved from the specified array.
     The array is assumed to contain a sufficient number of elements to
     populate the vector. It may contain more; extras are ignored.
     @param vals Array of element values.
     */
    explicit QVector3(const T *vals);

    /**
     Creates a vector by copying a source vector.
     @param src Vector to copy.
     */
    QVector3(const QVector<T, 3>& src);

    /**
     Creates a vector by copying a source vector in 2-D space and an extra
     homogeneous coordinate.
     @param src Vector to copy.
     @param homogeneous Terminal element value.
     */
    explicit QVector3(const QVector<T, 2>& src, T homogeneous);

    /**
     Creates a vector by copying a source vector in 4-D space, omitting the
     last element.
     @param src Vector to copy.
     */
    explicit QVector3(const QVector<T, 4>& src);

    /**
     Creates a vector from its individually-enumerated components.
     @param a First element.
     @param b Second element.
     @param c Third element.
     */
    QVector3(T a, T b, T c);

    /**
     Gets the cross-product of this vector and another.
     @param other Other vector.
     @return Cross-product.
     */
    QVector3<T> Cross(const QVector3<T>& other) const;

    const static QVector3<T> E1;
    const static QVector3<T> E2;
    const static QVector3<T> E3;
};

/* ------------------------------------------------------------------------- */

template<typename T> const QVector3<T> QVector3<T>::E1(1.0f, 0.0f, 0.0f);
template<typename T> const QVector3<T> QVector3<T>::E2(0.0f, 1.0f, 0.0f);
template<typename T> const QVector3<T> QVector3<T>::E3(0.0f, 0.0f, 1.0f);

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T>::QVector3()
  : QVector<T, 3>() {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T>::QVector3(T val)
  : QVector<T, 3>(val) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T>::QVector3(T first_vals,
                             T last_val)
  : QVector<T, 3>(first_vals, last_val) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T>::QVector3(const T *vals)
  : QVector<T, 3>(vals) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T>::QVector3(const QVector<T, 3>& src)
  : QVector<T, 3>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T>::QVector3(const QVector<T, 2>& src, T homogeneous)
  : QVector<T, 3>(src, homogeneous) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T>::QVector3(const QVector<T, 4>& src)
  : QVector<T, 3>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T>::QVector3(T a, T b, T c) {
  (*this)[0] = a;
  (*this)[1] = b;
  (*this)[2] = c;
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector3<T> QVector3<T>::Cross(const QVector3<T>& other) const {
  QVector3<T> cross;

  cross[0] = (*this)[1] * other[2] - (*this)[2] * other[1];
  cross[1] = (*this)[2] * other[0] - (*this)[0] * other[2];
  cross[2] = (*this)[0] * other[1] - (*this)[1] * other[0];

  return cross;
}

/* ------------------------------------------------------------------------- */

}

#endif
