#ifndef BOX_H
#define BOX_H

#include "QMath.h"
#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 An n-dimensional, axis aligned box structure.
 */
template<class T, int ndims> class Box {
  public:
    /**
     Creates a unit box situated at the origin.
     */
    Box();

    /**
     Creates a box situated at the specified position.
     */
    Box(const QVector<T, ndims>& max,
        const QVector<T, ndims>& min);

    /**
     Gets the position of the minimum corner of the box.
     @return Minimum corner position
     */
    const QVector<T, ndims>& GetMinimum() const;

    /**
     Gets the position of the maximum corner of the box.
     @return Maximum corner position
     */
    const QVector<T, ndims>& GetMaximum() const;

    /**
     Sets the position of the minimum corner of the box.
     @param min New position
     */
    void SetMinimum(const QVector<T, ndims>& min);

    /**
     Sets the position of the maximum corner of the box.
     @param max New position
     */
    void SetMaximum(const QVector<T, ndims>& max);

    /**
     Gets the length of the box's diagonal.
     @return Diagonal length
     */
    T GetDiagonalLength() const;

    /**
     Gets the center point of this box.
     @return Box's center
     */
    QVector<T, ndims> GetCenter() const;

    /**
     Gets the size of this box.
     @return Box's size
     */
    QVector<T, ndims> GetSize() const;

    bool Contains(const QVector<T, ndims>& point) const;

    int GetMinimumDimension() const;
    int GetMaximumDimension() const;

  private:
    /** Maximum corner */
    QVector<T, ndims> max;

    /** Minimum corner */
    QVector<T, ndims> min;

    /** Size, derived for quick access */
    QVector<T, ndims> size;
};

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
Box<T, ndims>::Box()
  : max((T) 1),
    min((T) -1),
    size(max - min) {
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
Box<T, ndims>::Box(const QVector<T, ndims>& max,
                   const QVector<T, ndims>& min)
  : max(max),
    min(min),
    size(max - min) {
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
const QVector<T, ndims>& Box<T, ndims>::GetMaximum() const {
  return max;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
const QVector<T, ndims>& Box<T, ndims>::GetMinimum() const {
  return min;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void Box<T, ndims>::SetMinimum(const QVector<T, ndims>& min) {
  this->min = min;
  size = max - min;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void Box<T, ndims>::SetMaximum(const QVector<T, ndims>& max) {
  this->max = max;
  size = max - min;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T Box<T, ndims>::GetDiagonalLength() const {
  return (max - min).GetLength();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims> Box<T, ndims>::GetCenter() const {
  return (max + min) / (T) 2;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims> Box<T, ndims>::GetSize() const {
  return size;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
bool Box<T, ndims>::Contains(const QVector<T, ndims>& point) const {
  for (int d = 0; d < ndims; ++d) {
    if (point[d] < min[d] || point[d] > max[d]) {
      return false;
    }
  }

  return true;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
int Box<T, ndims>::GetMaximumDimension() const {
  int dmin = 0;
  for (int d = 1; d < ndims; ++d) {
    if (size[d] > size[dmin]) {
      dmin = d;
    }
  }
  return dmin;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
int Box<T, ndims>::GetMinimumDimension() const {
  int dmin = 0;
  for (int d = 1; d < ndims; ++d) {
    if (size[d] < size[dmin]) {
      dmin = d;
    }
  }
  return dmin;
}

/* ------------------------------------------------------------------------- */

}

#endif
