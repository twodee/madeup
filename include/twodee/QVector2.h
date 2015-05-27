#ifndef QVECTOR2_H
#define QVECTOR2_H

#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A three-dimensional vector.
 */
template<class T> class QVector2 : public QVector<T, 2> {
  public:
    /**
     Creates a vector. Its initial values are undefined.
     */
    QVector2();

    /**
     Creates a vector, all of whose elements are the specified value.
     @param val Initial value of all elements.
     */
    explicit QVector2(T val);

    /**
     Creates a vector whose elements are retrieved from the specified array.
     The array is assumed to contain a sufficient number of elements to
     populate the vector. It may contain more; extras are ignored.
     @param vals Array of element values.
     */
    explicit QVector2(const T *vals);

    /**
     Creates a vector by copying a source vector.
     @param src Vector to copy.
     */
    QVector2(const QVector<T, 2>& src);

    /**
     Creates a vector by copying a source vector in 1-D space and an extra
     homogeneous coordinate.
     @param src Vector to copy.
     @param homogeneous Terminal element value.
     */
    QVector2(const QVector<T, 1>& src, T homogeneous);

    /**
     Creates a vector by copying a source vector in 3-D space, omitting the
     last element.
     @param src Vector to copy.
     */
    explicit QVector2(const QVector<T, 3>& src);

    /**
     Creates a vector from its individually-enumerated components.
     @param a First element.
     @param b Second element.
     */
    QVector2(T a, T b);
};

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector2<T>::QVector2()
  : QVector<T, 2>() {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector2<T>::QVector2(T val)
  : QVector<T, 2>(val) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector2<T>::QVector2(const T *vals)
  : QVector<T, 2>(vals) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector2<T>::QVector2(const QVector<T, 2>& src)
  : QVector<T, 2>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector2<T>::QVector2(const QVector<T, 1>& src, T homogeneous)
  : QVector<T, 2>(src, homogeneous) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector2<T>::QVector2(const QVector<T, 3>& src)
  : QVector<T, 2>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector2<T>::QVector2(T a, T b) {
  (*this)[0] = a;
  (*this)[1] = b;
}

/* ------------------------------------------------------------------------- */

}

#endif
