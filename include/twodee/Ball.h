#ifndef BALL_H
#define BALL_H

#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 An n-dimensional ball structure.
 */
template<class T, int ndims> class Ball {
  public:
    /**
     Creates a unit ball situated at the origin.
     */
    Ball();

    /**
     Creates a ball situated at the specified position with the specified
     radius.
     */
    Ball(T radius,
         const QVector<T, ndims>& center);

    /**
     Gets the ball's radius.
     @return Radius
     */
    T GetRadius() const;

    /**
     Gets the ball's center.
     @return Center
     */
    const QVector<T, ndims>& GetCenter() const;

    /**
     Determines if the specified point falls within the bounds of the sphere.
     @param point Location to check if inside sphere
     */
    bool Contains(const QVector<T, ndims>& point) const;

  private:
    /** Radius of ball */
    T radius;

    /** Center position of ball */
    QVector<T, ndims> center;
};

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
Ball<T, ndims>::Ball()
  : radius(1),
    center((T) 0) {
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
Ball<T, ndims>::Ball(T radius,
                     const QVector<T, ndims>& center)
  : radius(radius),
    center(center) {
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T Ball<T, ndims>::GetRadius() const {
  return radius;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
const QVector<T, ndims>& Ball<T, ndims>::GetCenter() const {
  return center;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
bool Ball<T, ndims>::Contains(const QVector<T, ndims>& point) const {
  return (point - center).GetLength() <= radius;
}

/* ------------------------------------------------------------------------- */

}

#endif
