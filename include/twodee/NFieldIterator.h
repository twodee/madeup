#ifndef NFIELDITERATOR_H
#define NFIELDITERATOR_H

#include "twodee/QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 An iterator for marching through n-dimensional fields.
 */
template<int ndims>
class NFieldIterator : public QVector<int, ndims> {
  public:
    /**
     Creates an iterator for a field with the specified dimensions, starting
     at the specified start position, and jumping by the specified delta.
     */
    NFieldIterator(const QVector<int, ndims>& exclusive_max,
                   const QVector<int, ndims>& min = (QVector<int, ndims>)(0),
                   const QVector<int, ndims>& delta = (QVector<int, ndims>)(1));

    /**
     Checks whether there's a next position to jump to still within the field.
     @returns True if there's a next position, false otherwise.
     */
    bool HasNext() const;

    /**
     Move to the next position in the field, jumping by the specified delta.
     */
    void Next();

    /**
     Restore the iterator to begin traversing at the originally specified start
     position.
     */
    void Reset();

  private:
    /** The starting position, inclusive */
    QVector<int, ndims> min;

    /** The amount to jump on each iteration */
    QVector<int, ndims> delta;

    QVector<int, ndims> stops;
    QVector<int, ndims> max;
};

/* ------------------------------------------------------------------------- */

template<int ndims>
NFieldIterator<ndims>::NFieldIterator(const QVector<int, ndims>& exclusive_max,
                                      const QVector<int, ndims>& min,
                                      const QVector<int, ndims>& delta)
  : QVector<int, ndims>(min),
    min(min),
    delta(delta) {
  for (int d = 0; d < ndims; ++d) {
    stops[d] = min[d] + delta[d] * (int) ceilf((exclusive_max[d] - min[d]) / (float) delta[d]);
    max[d] = stops[d] - delta[d];
  }
  (*this)[0] -= delta[0];
}

/* ------------------------------------------------------------------------- */

template<int ndims>
inline bool NFieldIterator<ndims>::HasNext() const {
  /* return (*this)[ndims - 1] != max[ndims - 1]; */
  bool is_last = true;
  for (int d = 0; d < ndims; ++d) {
    is_last = is_last && (*this)[d] == max[d]; 
  }
  return !is_last;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
inline void NFieldIterator<ndims>::Next() {
  (*this)[0] += delta[0];
  for (int d = 1; d < ndims && (*this)[d - 1] == stops[d - 1]; ++d) {
    (*this)[d - 1] = min[d - 1];
    (*this)[d] += delta[d];
  }
}

/* ------------------------------------------------------------------------- */

template<int ndims>
inline void NFieldIterator<ndims>::Reset() {
  for (int d = 0; d < ndims; ++d) {
    (*this)[d] = min[d];
  }
}

/* ------------------------------------------------------------------------- */

}

#endif
