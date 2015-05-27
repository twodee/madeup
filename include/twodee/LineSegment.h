#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "MessagedException.h"
#include "QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
class LineSegmentIterator;

/* ------------------------------------------------------------------------- */

/**
 A line segment in n-dimensional space.
 */
template<class T, int ndims> class LineSegment {
  public:
    /**
     Creates a new line segment. Endpoints are initially unspecified.
     */
    LineSegment();

    /**
     Creates a new line between the specified endpoints.
     @param a One endpoint
     @param b Other endpoint
     */
    LineSegment(const QVector<T, ndims>& a,
                const QVector<T, ndims>& b);

    /**
     Gets the specified endpoint.
     @param i 0 for first endpoint, 1 for second
     */
    const QVector<T, ndims>& operator[](int i) const;

    /**
     Gets the specified endpoint.
     @param i 0 for first endpoint, 1 for second
     */
    QVector<T, ndims>& operator[](int i);

    /**
     Gets an iterator to discretely march through this line segment.
     @return Iterator for stepping along line segment
     */
    LineSegmentIterator<T, ndims> GetIterator() const;

  private:
    /** Endpoints */
    QVector<T, ndims> ends[2];
};

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
LineSegment<T, ndims>::LineSegment() {
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
LineSegment<T, ndims>::LineSegment(const QVector<T, ndims>& a,
                                   const QVector<T, ndims>& b) {
  ends[0] = a;
  ends[1] = b;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
const QVector<T, ndims>& LineSegment<T, ndims>::operator[](int i) const {
  assert(i == 0 || i == 1); 
  return i == 0 ? ends[0] : ends[1];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
QVector<T, ndims>& LineSegment<T, ndims>::operator[](int i) {
  assert(i == 0 || i == 1); 
  return i == 0 ? ends[0] : ends[1];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
LineSegmentIterator<T, ndims> LineSegment<T, ndims>::GetIterator() const {
  return LineSegmentIterator<T, ndims>(*this);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims> class LineSegmentIterator : public QVector<int, ndims> {
  public:
    /**
     Creates a new iterator to march along the specified line segment.
     @param segment Line segment along which to march
     */
    LineSegmentIterator(const LineSegment<T, ndims>& segment);

    /**
     Checks whether another step can be taken on line.
     @return True if another step can be taken without exiting segment
     */
    bool HasNext() const;

    /**
     Steps along line segment.
     */
    void Next();

  private:
};

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
LineSegmentIterator<T, ndims>::LineSegmentIterator(const LineSegment<T, ndims>& segment) {
  throw MessagedException("Iterators for n-dimensions not supported");
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
bool LineSegmentIterator<T, ndims>::HasNext() const {
  throw MessagedException("Iterators for n-dimensions not supported");
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void LineSegmentIterator<T, ndims>::Next() {
  throw MessagedException("Iterators for n-dimensions not supported");
}

/* ------------------------------------------------------------------------- */

template<class T> class LineSegmentIterator<T, 2> : public QVector<int, 2> {
  public:
    /**
     Creates a new iterator to march along the specified line segment.
     @param segment Line segment along which to march
     */
    LineSegmentIterator(const LineSegment<T, 2>& segment);

    /**
     Checks whether another step can be taken on line.
     @return True if another step can be taken without exiting segment
     */
    bool HasNext() const;

    /**
     Steps along line segment.
     */
    void Next();

  private:
    /** Segment to step along */
    LineSegment<int, 2> segment;

    QVector<int, 2> diff;
    QVector<int, 2> step;

    int fraction;
    int f;
    int s;
};

/* ------------------------------------------------------------------------- */

template<class T>
LineSegmentIterator<T, 2>::LineSegmentIterator(const LineSegment<T, 2>& ssegment) {

  for (int i = 0; i < 2; ++i) {
    if (ssegment[0][i] < ssegment[1][i]) {
      this->segment[0][i] = (int) ceil(ssegment[0][i]);
      this->segment[1][i] = (int) floor(ssegment[1][i]);
    } else {
      this->segment[0][i] = (int) floor(ssegment[0][i]);
      this->segment[1][i] = (int) ceil(ssegment[1][i]);
    }
  }

  diff = this->segment[1] - this->segment[0];

  // TODO?
  for (int i = 0; i < 2; ++i) {
    if (diff[i] < 0) {
      diff[i] = -diff[i];
      step[i] = -1;
    } else {
      step[i] = 1;
    }

    diff[i] <<= 1;
  }

  // Figure out which dimension moves faster.
  f = diff[0] > diff[1] ? 0 : 1;
  s = 1 - f;

  fraction = diff[s] - (diff[f] >> 1);

  for (int i = 0; i < 2; ++i) {
    (*this)[i] = this->segment[0][i];
  }
}

/* ------------------------------------------------------------------------- */

template<class T>
bool LineSegmentIterator<T, 2>::HasNext() const {
  return (*this)[f] != this->segment[1][f];
}

/* ------------------------------------------------------------------------- */

template<class T>
void LineSegmentIterator<T, 2>::Next() {
  if (fraction >= 0) {
    (*this)[s] += step[s];
    fraction -= diff[f];
  }
  (*this)[f] += step[f];
  fraction += diff[s];
}

/* ------------------------------------------------------------------------- */

}

#endif
