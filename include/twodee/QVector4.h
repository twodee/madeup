#ifndef QVECTOR4_H
#define QVECTOR4_H

#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A four-dimensional vector.
 */
template<class T> class QVector4 : public QVector<T, 4> {
  public:
    /**
     Creates a vector. Its initial values are undefined.
     */
    QVector4();

    /**
     Creates a vector, all of whose elements are the specified value.
     @param val Initial value of all elements.
     */
    explicit QVector4(T val);

    /**
     Creates a vector, all of whose non-terminal elements are the first
     specified value, and whose terminal element is the second specified value.
     This may be useful for vectors in homogeneous space.
     @param first_vals Initial value of all non-terminal elements.
     @param last_val Initial value of all terminal element.
     */
    QVector4(T first_vals, T last_val);

    /**
     Creates a vector whose elements are retrieved from the specified array.
     The array is assumed to contain a sufficient number of elements to
     populate the vector. It may contain more; extras are ignored.
     @param vals Array of element values.
     */
    explicit QVector4(const T *vals);

    /**
     Creates a vector by copying a source vector.
     @param src Vector to copy.
     */
    QVector4(const QVector<T, 4>& src);

    /**
     Creates a vector by copying a source vector in 3-D space and an extra
     homogeneous coordinate.
     @param src Vector to copy.
     @param homogeneous Terminal element value.
     */
    explicit QVector4(const QVector<T, 3>& src, T homogeneous);

    /**
     Creates a vector from its individually-enumerated components.
     @param a First element.
     @param b Second element.
     @param c Third element.
     @param d Fourth element.
     */
    QVector4(T a, T b, T c, T d);
};

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector4<T>::QVector4()
  : QVector<T, 4>() {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector4<T>::QVector4(T val)
  : QVector<T, 4>(val) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector4<T>::QVector4(T first_vals,
                             T last_val)
  : QVector<T, 4>(first_vals, last_val) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector4<T>::QVector4(const T *vals)
  : QVector<T, 4>(vals) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector4<T>::QVector4(const QVector<T, 4>& src)
  : QVector<T, 4>(src) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector4<T>::QVector4(const QVector<T, 3>& src, T homogeneous)
  : QVector<T, 4>(src, homogeneous) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline QVector4<T>::QVector4(T a, T b, T c, T d) {
  (*this)[0] = a;
  (*this)[1] = b;
  (*this)[2] = c;
  (*this)[3] = d;
}

/* ------------------------------------------------------------------------- */

}

#endif
