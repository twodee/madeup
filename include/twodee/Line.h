#ifndef LINE_H
#define LINE_H

#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

template<typename T, int ndims> class Line {
  public:
    Line();
    Line(const QVector<T, ndims>& point, const QVector<T, ndims>& direction);

    const QVector<T, ndims>& GetPoint() const;
    const QVector<T, ndims>& GetDirection() const;

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

}

#endif
