#ifndef VECTOR_H
#define VECTOR_H

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
template<class T> class Vector {
  public:
    /**
     Creates a vector. Its initial values are undefined.
     */
    Vector(int ndims = 0);

    /**
     Creates a vector, all of whose elements are the specified value.
     @param val Initial value of all elements.
     */
    explicit Vector(int ndims, T val);

    /**
     Creates a vector, all of whose non-terminal elements are the first
     specified value, and whose terminal element is the second specified value.
     This may be useful for vectors in homogeneous space.
     @param first_vals Initial value of all non-terminal elements.
     @param last_val Initial value of all terminal element.
     */
    Vector(int ndims, T first_vals, T last_val);

    /**
     Creates a vector whose elements are retrieved from the specified array.
     The array is assumed to contain a sufficient number of elements to
     populate the vector. It may contain more; extras are ignored.
     @param vals Array of element values.
     */
    explicit Vector(int ndims, const T *vals);

    /**
     Creates a vector by copying a source vector.
     @param src Vector to copy.
     */
    Vector(const Vector<T>& src);

    ~Vector();

    /**
     Gets element at specified index.
     @param i Index of element to retrieve.
     @return Element at specified index.
     */
    T& operator[](int i);

    Vector<T>& operator=(const Vector<T>& other);

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
    T Dot(const Vector<T>& other) const;

    /**
     Makes this vector have length 1, while maintaining its direction.
     */
    void Normalize();

    /**
     Gets the product of all elements.
     @return Product of elements.
     */
    T SelfProduct() const;

    /**
     Adds another vector to this vector and gets the result.
     @param other Vector to add.
     @return Sum vector.
     */
    Vector<T>& operator+=(const Vector<T>& other);

    /**
     Subtracts another vector from this vector and gets the result.
     @param other Vector to subtract.
     @return Difference vector.
     */
    Vector<T>& operator-=(const Vector<T>& other);

    /**
     Multiplies another vector by this vector, component-wise, and gets the
     result.
     @param other Vector to multiply.
     @return Product vector.
     */
    Vector<T>& operator*=(const Vector<T>& other);

    /**
     Divides this vector by another vector, component-wise, and gets the
     result.
     @param other Vector by which to divide.
     @return Quotient vector.
     */
    Vector<T>& operator/=(const Vector<T>& other);

    /**
     Adds a scalar to each component of this vector and gets the result.
     @param scalar Scalar to add.
     @return Sum vector.
     */
    Vector<T>& operator+=(T scalar);

    /**
     Subtracts a scalar from each component of this vector and gets the result.
     @param scalar Scalar to subtract.
     @return Difference vector.
     */
    Vector<T>& operator-=(T scalar);

    /**
     Multiplies a scalar by each component of this vector and gets the result.
     @param scalar Scalar to multiply.
     @return Product vector.
     */
    Vector<T>& operator*=(T scalar);

    /**
     Divides each component of this vector by a scalar and gets the result.
     @param scalar Scalar by which to divide.
     @return Quotient vector.
     */
    Vector<T>& operator/=(T scalar);

    Vector<T> Abs() const;

    /**
     Casts the vector to a vector of another type.
     */
    template<class U>
    operator Vector<U> () const;

    int GetDimensionCount() const {
      return ndims;
    }

  private:
    /** The elements */
    int ndims;
    T *data;
};

/* ------------------------------------------------------------------------- */

template<class T>
std::ostream& operator<<(std::ostream& stream,
                         const Vector<T>& v) {
  stream << v[0];
  for (int i = 1; i < v.GetDimensionCount(); ++i) {
    stream << ' ' << v[i];
  }
  return stream;
}

/* ------------------------------------------------------------------------- */

template<class T>
std::istream& operator>>(std::istream& stream,
                         Vector<T>& v) {
  stream >> v[0];
  for (int i = 1; i < v.GetDimensionCount(); ++i) {
    stream >> v[i];
  }
  return stream;
}

/* ------------------------------------------------------------------------- */

template<class T>
bool operator==(const Vector<T>& a,
                const Vector<T>& b) {
  if (a.GetDimensionCount() != b.GetDimensionCount()) {
    return false;
  }

  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator+(const Vector<T>& a,
                    const Vector<T>& b) {
  assert(a.GetDimensionCount() == b.GetDimensionCount());
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
    ans[i] = a[i] + b[i];
  }
  return ans;
}

/* ------------------------------------------------------------------------- */

template<class T>
Vector<T> operator%(const Vector<T>& a, const Vector<T>& b) {
  assert(a.GetDimensionCount() == b.GetDimensionCount());
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator-(const Vector<T>& a,
                    const Vector<T>& b) {
  assert(a.GetDimensionCount() == b.GetDimensionCount());
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator*(const Vector<T>& a,
                            const Vector<T>& b) {
  assert(a.GetDimensionCount() == b.GetDimensionCount());
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator/(const Vector<T>& a,
                            const Vector<T>& b) {
  assert(a.GetDimensionCount() == b.GetDimensionCount());
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator+(const Vector<T>& a,
                    T scalar) {
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator-(const Vector<T>& a,
                    T scalar) {
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator*(const Vector<T>& a,
                    T scalar) {
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator/(const Vector<T>& a,
                    T scalar) {
  Vector<T> ans(a.GetDimensionCount());
  for (int i = 0; i < a.GetDimensionCount(); ++i) {
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
template<class T>
Vector<T> operator+(T scalar,
                    const Vector<T>& a) {
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
template<class T>
Vector<T> operator-(T scalar,
                    const Vector<T>& a) {
  return Vector<T>(scalar) - a;
}

/* ------------------------------------------------------------------------- */

/**
 Gets the vector which representing this vector with the specified scalar value
 multiplied by each component.
 @param scalar Scalar value multiplied by each component.
 @param a Vector operand.
 @return Product vector.
 */
template<class T>
Vector<T> operator*(T scalar,
                    const Vector<T>& a) {
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
template<class T>
Vector<T> operator/(T scalar,
                    const Vector<T>& a) {
  return Vector<T>(a.GetDimensions(), scalar) / a;
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>::Vector(int ndims) :
  ndims(ndims),
  data(new T[ndims]) {
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>::Vector(int ndims, T val) :
  ndims(ndims),
  data(new T[ndims]) {
  assert(ndims > 0);
  for (int i = 0; i < ndims; i++) {
    data[i] = val;
  }
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>::Vector(int ndims, T first_vals, T last_val) :
  ndims(ndims),
  data(new T[ndims]) {
  assert(ndims > 0);
  for (int i = 0; i <= ndims - 2; i++) {
    data[i] = first_vals;
  }
  data[ndims - 1] = last_val;
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>::Vector(int ndims, const T *vals) :
  ndims(ndims),
  data(new T[ndims]) {
  assert(ndims > 0);
  for (int i = 0; i < ndims; i++) {
    data[i] = vals[i];
  }
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>::Vector(const Vector<T>& src) :
  ndims(src.GetDimensionCount()),
  data(new T[ndims]) {
  memcpy(data, src.data, sizeof(T) * ndims);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline T& Vector<T>::operator[](int i) {
  assert(i >= 0 && i < ndims);
  return data[i];
}

/* ------------------------------------------------------------------------- */

template<class T>
inline const T& Vector<T>::operator[](int i) const {
  assert(i >= 0 && i < ndims);
  return data[i];
}

/* ------------------------------------------------------------------------- */

template<class T>
inline T Vector<T>::GetSquaredLength() const {
  return Dot(*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline T Vector<T>::GetLength() const {
  return (T) sqrt(GetSquaredLength());
}

/* ------------------------------------------------------------------------- */

template<class T>
inline T Vector<T>::Dot(const Vector<T>& other) const {
  assert(GetDimensionCount() == other.GetDimensionCount());
  T sum_products = 0;
  for (int i = 0; i < ndims; ++i) {
    sum_products += (*this)[i] * other[i];
  }
  return sum_products;
}

/* ------------------------------------------------------------------------- */

template<class T>
inline void Vector<T>::Normalize() {
  *this /= GetLength();
}

/* ------------------------------------------------------------------------- */

template<class T>
T Vector<T>::SelfProduct() const {
  T product = 1;
  for (int i = 0; i < ndims; ++i) {
    product *= (*this)[i];
  }
  return product;
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>& Vector<T>::operator+=(const Vector<T>& other) {
  assert(GetDimensionCount() == other.GetDimensionCount());
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] += other[i];
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>& Vector<T>::operator-=(const Vector<T>& other) {
  assert(GetDimensionCount() == other.GetDimensionCount());
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] -= other[i];
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>& Vector<T>::operator*=(const Vector<T>& other) {
  assert(GetDimensionCount() == other.GetDimensionCount());
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] *= other[i];
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>& Vector<T>::operator/=(const Vector<T>& other) {
  assert(GetDimensionCount() == other.GetDimensionCount());
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] /= other[i];
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>& Vector<T>::operator+=(T scalar) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] += scalar;
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>& Vector<T>::operator-=(T scalar) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] -= scalar;
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>& Vector<T>::operator*=(T scalar) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] *= scalar;
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
inline Vector<T>& Vector<T>::operator/=(T scalar) {
  for (int i = 0; i < ndims; ++i) {
    (*this)[i] /= scalar;
  }
  return (*this);
}

/* ------------------------------------------------------------------------- */

template<class T>
Vector<T> Vector<T>::Abs() const {
  Vector<T> positive;
  for (int d = 0; d < ndims; ++d) {
    positive[d] = (*this)[d] >= 0 ? (*this)[d] : -(*this)[d];
  }
  return positive;
}

/* ------------------------------------------------------------------------- */

template<class T>
template<class U> Vector<T>::operator Vector<U> () const {
   Vector<U> dst(GetDimensionCount());
   Utilities::ConvertArray(&dst[0], &(*this)[0], ndims);
   return dst;
}

/* ------------------------------------------------------------------------- */

template<class T>
Vector<T>::~Vector() {
  delete[] data;
  data = NULL;
}

/* ------------------------------------------------------------------------- */

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
  if (this == &other) {
    return *this;
  }

  ndims = other.GetDimensionCount(); 
  delete[] data;
  data = new T[ndims];
  memcpy(data, other.data, sizeof(T) * ndims);
  return *this;
}

/* ------------------------------------------------------------------------- */

}

#endif
