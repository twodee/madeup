#ifndef LINE_H
#define LINE_H

#include "QVector.h"
#include "QVector3.h"

namespace td {

/* ------------------------------------------------------------------------- */

template<typename T, int ndims> class Line {
  public:
    Line();
    Line(const QVector<T, ndims>& point, const QVector<T, ndims>& direction);

    const QVector<T, ndims>& GetPoint() const;
    const QVector<T, ndims>& GetDirection() const;

    QVector<T, 3> ClosestPointTo(const Line<T, ndims> &that) const;

  private:
    QVector<T, ndims> point;
    QVector<T, ndims> direction;
};

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
Line<T, ndims>::Line() {
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
Line<T, ndims>::Line(const QVector<T, ndims>& point, const QVector<T, ndims>& direction) :
  point(point),
  direction(direction) {
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
const QVector<T, ndims>& Line<T, ndims>::GetPoint() const {
  return point;
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
const QVector<T, ndims>& Line<T, ndims>::GetDirection() const {
  return direction;
}

/* ------------------------------------------------------------------------- */

template<typename T, int ndims>
QVector<T, 3> Line<T, ndims>::ClosestPointTo(const Line<T, ndims> &that) const {
  assert(ndims == 3);

  QVector3<T> this_direction = this->GetDirection();
  QVector3<T> that_direction = that.GetDirection();
  QVector3<T> this_point = this->GetPoint();
  QVector3<T> that_point = that.GetPoint();

  T na = that_direction.Cross(this_point - that_point).Dot(this_direction.Cross(that_direction));
  T d = this_direction.Cross(that_direction).Dot(this_direction.Cross(that_direction));
  QVector3<T> a0 = this_point + (na / d) * this_direction;

  return a0;
}

/* ------------------------------------------------------------------------- */

}

#endif
