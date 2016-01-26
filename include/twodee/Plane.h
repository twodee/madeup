#ifndef PLANE_H
#define PLANE_H

#include "Line.h"
#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

template<typename T, int ndims> class Plane {
  public:
    Plane();
    Plane(const QVector<T, ndims>& point, const QVector<T, ndims>& normal);
    
    const QVector<T, ndims>& GetNormal() const;
    const QVector<T, ndims>& GetPoint() const;

    bool Intersect(const Line<T, ndims>& line, QVector<T, ndims>& intersection_point) const;
    T DistanceTo(const QVector<T, ndims>& point) const;

  protected:
    QVector<T, ndims> point;
    QVector<T, ndims> normal;
};

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
Plane<T, ndims>::Plane() {
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
Plane<T, ndims>::Plane(const QVector<T, ndims>& point, const QVector<T, ndims>& normal) :
  point(point),
  normal(normal) {
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
const QVector<T, ndims>& Plane<T, ndims>::GetNormal() const {
  return normal;
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
const QVector<T, ndims>& Plane<T, ndims>::GetPoint() const {
  return point;
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
bool Plane<T, ndims>::Intersect(const Line<T, ndims>& line, QVector<T, ndims>& intersection_point) const {
  T dot = this->GetNormal().Dot(line.GetDirection()); 

  if (dot == 0) {
    return false;
  } else {
    QVector<T, ndims> diff = this->GetPoint() - line.GetPoint();
    T alpha = this->GetNormal().Dot(diff) / dot;
    intersection_point = line.GetPoint() + line.GetDirection() * alpha;
    return true;
  }
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
T Plane<T, ndims>::DistanceTo(const QVector<T, ndims>& point) const {
  return normal.Dot(point - this->point);
}

/* ------------------------------------------------------------------------- */

}

#endif
