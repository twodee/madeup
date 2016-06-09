#ifndef TD_QVECTOR_H
#define TD_QVECTOR_H

#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>

#include "MessagedException.h"
#include "Utilities.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 An n-dimensional tuple.
 */
template<class T, int ndims> class QVector {
  public:
    /**
     Creates a vector. Its initial values are undefined.
     */
    QVector();

    /**
     Creates a vector, all of whose elements are the specified value.
     @param val Initial value of all elements.
     */
    explicit QVector(T val);

    /**
     Creates a vector, all of whose non-terminal elements are the first
     specified value, and whose terminal element is the second specified value.
     This may be useful for vectors in homogeneous space.
     @param first_vals Initial value of all non-terminal elements.
     @param last_val Initial value of all terminal element.
     */
    QVector(T first_vals, T last_val);

    /**
     Creates a vector whose elements are retrieved from the specified array.
     The array is assumed to contain a sufficient number of elements to
     populate the vector. It may contain more; extras are ignored.
     @param vals Array of element values.
     */
    explicit QVector(const T *vals);

    /**
     Creates a vector by copying a source vector.
     @param src Vector to copy.
     */
    QVector(const QVector<T, ndims>& src);

    /**
     Creates a vector by copying a source vector in a space with one less
     dimension and an extra homogeneous coordinate.
     @param src Vector to copy.
     @param homogeneous Terminal element value.
     */
    explicit QVector(const QVector<T, ndims - 1>& src, T homogeneous);

    /**
     Creates a vector by copying a source vector in a space with one more
     dimension, omitting the last element.
     @param src Vector to copy.
     */
    explicit QVector(const QVector<T, ndims + 1>& src);

    /**
     Gets element at specified index.
     @param i Index of element to retrieve.
     @return Element at specified index.
     */
    T& operator[](int i);

    /**
     Gets element at specified index.
     @param i Index of element to retrieve.
     @return Element at specified index.
     */
    const T& operator[](int i) const;

    /**
     Gets vector's squared length.
     @return Vector's squared length.
     */
    T GetSquaredLength() const;

    /**
     Gets vector's length.
     @return Vector's length.
     */
    T GetLength() const;

    /**
     Gets the dot product between this vector and another.
     @param other Other vector.
     @return Dot product.
     */
    T Dot(const QVector<T, ndims>& other) const;

    /**
     Makes this vector have length 1, while maintaining its direction.
     */
    void Normalize();

    /**
     Gets the product of all elements.
     @return Product of elements.
     */
    T SelfProduct() const;

    T GetMinimum() const;
    T GetMaximum() const;

    /**
     Adds another vector to this vector and gets the result.
     @param other Vector to add.
     @return Sum vector.
     */
    QVector<T, ndims>& operator+=(const QVector<T, ndims>& other);

    /**
     Subtracts another vector from this vector and gets the result.
     @param other Vector to subtract.
     @return Difference vector.
     */
    QVector<T, ndims>& operator-=(const QVector<T, ndims>& other);

    /**
     Multiplies another vector by this vector, component-wise, and gets the
     result.
     @param other Vector to multiply.
     @return Product vector.
     */
    QVector<T, ndims>& operator*=(const QVector<T, ndims>& other);

    /**
     Divides this vector by another vector, component-wise, and gets the
     result.
     @param other Vector by which to divide.
     @return Quotient vector.
     */
    QVector<T, ndims>& operator/=(const QVector<T, ndims>& other);

    /**
     Adds a scalar to each component of this vector and gets the result.
     @param scalar Scalar to add.
     @return Sum vector.
     */
    QVector<T, ndims>& operator+=(T scalar);

    /**
     Subtracts a scalar from each component of this vector and gets the result.
     @param scalar Scalar to subtract.
     @return Difference vector.
     */
    QVector<T, ndims>& operator-=(T scalar);

    /**
     Multiplies a scalar by each component of this vector and gets the result.
     @param scalar Scalar to multiply.
     @return Product vector.
     */
    QVector<T, ndims>& operator*=(T scalar);

    /**
     Divides each component of this vector by a scalar and gets the result.
     @param scalar Scalar by which to divide.
     @return Quotient vector.
     */
    QVector<T, ndims>& operator/=(T scalar);

    QVector<T, ndims> Abs() const;

    QVector<T, ndims> GetPerpendicular() const;

    QVector<T, ndims> GetReverse() const;

    T GetDistanceTo(const QVector<T, ndims> &other) const;

    /**
     Casts the vector to a vector of another type.
     */
    template<class U>
    operator QVector<U, ndims> () const;

  private:
    /** The elements */
    T data[ndims];
};

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
std::ostream& operator<<(std::ostream& stream,
                         const QVector<T, ndims>& v) {
  stream << v[0];
  for (int i = 1; i < ndims; ++i) {
    stream << ' ' << v[i];
  }
  return stream;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
std::istream& operator>>(std::istream& stream,
                         QVector<T, ndims>& v) {
  stream >> v[0];
  for (int i = 1; i < ndims; ++i) {
    stream >> v[i];
  }
  return stream;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
bool operator==(const QVector<T, ndims>& a,
                const QVector<T, ndims>& b) {
  for (int i = 0; i < ndims; ++i) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the sum of two vectors.
 @param a First operand.
 @param b Second operand.
 @return Sum vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator+(const QVector<T, ndims>& a,
                            const QVector<T, ndims>& b) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] + b[i];
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims> operator%(const QVector<T, ndims>& a, const QVector<T, ndims>& b) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] % b[i];
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the difference between two vectors.
 @param a First operand.
 @param b Second operand.
 @return Difference vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator-(const QVector<T, ndims>& a,
                            const QVector<T, ndims>& b) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] - b[i];
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the product of two vectors.
 @param a First operand.
 @param b Second operand.
 @return Product vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator*(const QVector<T, ndims>& a,
                            const QVector<T, ndims>& b) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] * b[i];
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the quotient of two vectors.
 @param a First operand.
 @param b Second operand.
 @return Quotient vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator/(const QVector<T, ndims>& a,
                            const QVector<T, ndims>& b) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] / b[i];
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector representing this vector with the specified scalar value
 added to each component.
 @param a Vector operand.
 @param scalar Scalar value added to each component.
 @return Sum vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator+(const QVector<T, ndims>& a,
                            T scalar) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] + scalar;
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector representing this vector with the specified scalar value
 subtracted from each component.
 @param a Vector operand.
 @param scalar Scalar value subtracted from each component.
 @return Difference vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator-(const QVector<T, ndims>& a,
                            T scalar) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] - scalar;
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector which representing this vector with the specified scalar value
 multiplied by each component.
 @param a Vector operand.
 @param scalar Scalar value multiplied by each component.
 @return Product vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator*(const QVector<T, ndims>& a,
                            T scalar) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] * scalar;
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector which representing this vector with the specified scalar value
 dividing each component.
 @param a Vector operand.
 @param scalar Scalar value dividing each component.
 @return Quotient vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator/(const QVector<T, ndims>& a,
                            T scalar) {
  QVector<T, ndims> ans;
  for (int i = 0; i < ndims; ++i) {
    ans[i] = a[i] / scalar;
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector representing this vector with the specified scalar value
 added to each component.
 @param scalar Scalar value added to each component.
 @param a Vector operand.
 @return Sum vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator+(T scalar,
                            const QVector<T, ndims>& a) {
  return a + scalar;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector representing this vector with the specified scalar value
 subtracted from each component.
 @param scalar Scalar value subtracted from each component.
 @param a Vector operand.
 @return Difference vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator-(T scalar,
                            const QVector<T, ndims>& a) {
  return QVector<T, ndims>(scalar) - a;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector which representing this vector with the specified scalar value
 multiplied by each component.
 @param scalar Scalar value multiplied by each component.
 @param a Vector operand.
 @return Product vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator*(T scalar,
                            const QVector<T, ndims>& a) {
  return a * scalar;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector which representing this vector with the specified scalar value
 dividing each component.
 @param scalar Scalar value dividing each component.
 @param a Vector operand.
 @return Quotient vector.
 */
template<class T, int ndims>
QVector<T, ndims> operator/(T scalar,
                            const QVector<T, ndims>& a) {
  return QVector<T, ndims>(scalar) / a;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>::QVector() {
  assert(ndims > 0);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>::QVector(T val) {
  assert(ndims > 0);
  for (int i = 0; i < ndims; i++) {
    data[i] = val;
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>::QVector(T first_vals, T last_val) {
  assert(ndims > 0);
  for (int i = 0; i <= ndims - 2; i++) {
    data[i] = first_vals;
  }
  data[ndims - 1] = last_val;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>::QVector(const T *vals) {
  assert(ndims > 0);
  for (int i = 0; i < ndims; i++) {
    data[i] = vals[i];
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>::QVector(const QVector<T, ndims>& src) {
  memcpy(data, src.data, sizeof(T) * ndims);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>::QVector(const QVector<T, ndims - 1>& src, T homogeneous) {
  memcpy(data, &(src[0]), sizeof(T) * (ndims - 1));
  data[ndims - 1] = homogeneous;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>::QVector(const QVector<T, ndims + 1>& src) {
  memcpy(data, &(src[0]), sizeof(T) * ndims);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline T& QVector<T, ndims>::operator[](int i) {
  assert(i >= 0 && i < ndims);
  return data[i];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline const T& QVector<T, ndims>::operator[](int i) const {
  assert(i >= 0 && i < ndims);
  return data[i];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline T QVector<T, ndims>::GetSquaredLength() const {
  return Dot(*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline T QVector<T, ndims>::GetLength() const {
  return (T) sqrt(GetSquaredLength());
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline T QVector<T, ndims>::Dot(const QVector<T, ndims>& other) const {
  T sum_products = 0;
  for (int i = 0; i < ndims; ++i) {
    sum_products += (*this)[i] * other[i];
  }
  return sum_products;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline void QVector<T, ndims>::Normalize() {
  *this /= GetLength();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T QVector<T, ndims>::SelfProduct() const {
  T product = 1;
  for (int i = 0; i < ndims; ++i) {
    product *= (*this)[i];
  }
  return product;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>& QVector<T, ndims>::operator+=(const QVector<T, ndims>& other) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] += other[i];
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>& QVector<T, ndims>::operator-=(const QVector<T, ndims>& other) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] -= other[i];
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>& QVector<T, ndims>::operator*=(const QVector<T, ndims>& other) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] *= other[i];
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>& QVector<T, ndims>::operator/=(const QVector<T, ndims>& other) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] /= other[i];
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>& QVector<T, ndims>::operator+=(T scalar) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] += scalar;
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>& QVector<T, ndims>::operator-=(T scalar) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] -= scalar;
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>& QVector<T, ndims>::operator*=(T scalar) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] *= scalar;
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
inline QVector<T, ndims>& QVector<T, ndims>::operator/=(T scalar) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] /= scalar;
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims> QVector<T, ndims>::Abs() const {
  QVector<T, ndims> positive;
  for (int d = 0; d < ndims; ++d) {
    positive[d] = (*this)[d] >= 0 ? (*this)[d] : -(*this)[d];
  }
  return positive;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
template<class U> QVector<T, ndims>::operator QVector<U, ndims> () const {
   QVector<U, ndims> dst;
   Utilities::ConvertArray(&dst[0], &(*this)[0], ndims);
   return dst;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims> QVector<T, ndims>::GetPerpendicular() const {
   /*
    This function takes the given vector and finds an arbitrary vector
    ans perpendicular to it.  The premise is as follows:
    
       We need dot(vec, ans) == 0 for perpendicularity.  In other words:
       vec[0] * ans[0] + vec[1] * ans[1] + vec[2] * ans[2] == 0
    
       We could say ans[1] and ans[2] are 1:
       vec[0] * ans[0] + vec[1] + vec[2] == 0
    
       This allows us to solve for ans[0], provided vec[0] != 0:
       ans[0] == -(vec[1] + vec[2]) / vec[0]
    
       If vec[0] is 0, then we could do similar things for the other two
       dimensions.
    
       If vec is the zero vector, nothing's perpendicular.
    */

  QVector<T, ndims> ans(1);

  int i;
  for (i = 0; i < ndims; i++) {
    if ((*this)[i] != 0) {
      break;
    }
  }

  if (i == ndims) {
    throw MessagedException("No perpendicular vector.");
  } else {
    T sum = 0;
    for (int j = 0; j < ndims; ++j) {
      if (j != i) {
        sum += (*this)[j];
      }
    }
    ans[i] = -sum / (*this)[i];
    ans.Normalize();
  }

  return ans;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims> QVector<T, ndims>::GetReverse() const {
  QVector<T, ndims> ans;
  for (int d = 0; d < ndims; ++d) {
    ans[d] = (*this)[ndims - 1 - d];
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T QVector<T, ndims>::GetDistanceTo(const QVector<T, ndims> &other) const {
  return (*this - other).GetLength(); 
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T QVector<T, ndims>::GetMaximum() const {
  T max = (*this)[0];
  for (int i = 1; i < ndims; ++i) {
    if ((*this)[i] > max) {
      max = (*this)[i];
    }
  }
  return max;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T QVector<T, ndims>::GetMinimum() const {
  T min = (*this)[0];
  for (int i = 1; i < ndims; ++i) {
    if ((*this)[i] < min) {
      min = (*this)[i];
    }
  }
  return min;
}

/* ------------------------------------------------------------------------- */

}

#endif
