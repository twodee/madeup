#ifndef CONTOURMARCHER_H
#define CONTOURMARCHER_H

#include <list>

#include "twodee/MessagedException.h"
#include "twodee/NFieldBase.h"
#include "twodee/NField.h"
#include "twodee/NFieldIterator.h"
#include "twodee/QVector.h"

namespace td {

/* ------------------------------------------------------------------------- */

template<class T, int ndims> class NField;

/* ------------------------------------------------------------------------- */

template<class T, int ndims> class ContourMarcher {
  public:
    static void GetIsocontour(const NField<T, ndims>& field,
                              T isovalue,
                              int& nprimitives,
                              float*& positions,
                              int channel = 0);
};

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void ContourMarcher<T, ndims>::GetIsocontour(const NField<T, ndims>& field,
                                             T isovalue,
                                             int& nprimitives,
                                             float*& positions,
                                             int channel) {
  throw MessagedException("Contouring for n-dimensions not supported.");
}

/* ------------------------------------------------------------------------- */

template<class T> class ContourMarcher<T, 3> {
  public:
    static void GetIsocontour(const NField<T, 3>& field,
                              T isovalue,
                              int& nprimitives,
                              float*& positions,
                              int channel = 0);
};

/* ------------------------------------------------------------------------- */

template<class T>
void ContourMarcher<T, 3>::GetIsocontour(const NField<T, 3>& field,
                                         T isovalue,
                                         int& nprimitives,
                                         float*& positions,
                                         int channel) {
  const int ndims = 3;
  const int ncorners = (1 << ndims);
  const int nedges = ncorners * ndims / 2;
  
  // Get static offsets from base to other corners for quick access.
  int offsets[ncorners];
  QVector<int, ndims> max(2);
  NFieldIterator<ndims> c(max);
  for (int i = 0; c.HasNext(); ++i) {
    c.Next();
    offsets[i] = field(c) - field(0);
  }

  // TODO? what does this do?
  QVector<int, ndims> edge_starts[nedges];
  for (int d = 0, ei = 0; d < ndims; ++d) {
    QVector<int, ndims> max(2);
    max[d] = 1;
    NFieldIterator<ndims> c(max);
    while (c.HasNext()) {
      c.Next();
      edge_starts[ei] = c;
      ++ei;
    }
  }

  // Now march. We stop at the penultimate locations because these are the
  // last valid bases of square/cube/etc. We don't know the number of
  // primitives ahead of time, so we keep a list.
  // TODO: But we do know the upper bound. Perhaps it'd be more efficient
  // to allocate a big-enough array?
  std::list<QVector<float, ndims> > position_list;
  QVector<float, ndims> intersections[nedges];
  c = NFieldIterator<ndims>(field.GetDimensions() - 1);
  while (c.HasNext()) {
    c.Next();

    // Keep a bit field to determine which corners are above/below isovalue. If
    // bit i is on, then corner i's value is above the isovalue.
    int configuration = 0;
    const T *base = &(field(c)[channel]);
    for (int ci = 0; ci < ncorners; ++ci) {
      T datum = *(base + offsets[ci]);
      configuration |= ((datum < isovalue) << ci);
    }

    // Find the where the contour hits the relevant edges.
    int edges_to_intersect = NFieldBase<ndims>::CASE_TO_EDGE[configuration];
    for (int ei = 0; ei < nedges; ++ei) {
      if (edges_to_intersect & (1 << ei)) {
        QVector<int, ndims> fore_position = c + edge_starts[ei];
        QVector<int, ndims> aft_position = fore_position;
        int d = ei / (nedges / ndims);
        aft_position[d] += 1;
        intersections[ei] = fore_position;
        T fore_value = field(fore_position)[channel];
        T aft_value = field(aft_position)[channel];
        float weight = (isovalue - fore_value) / (aft_value - fore_value);
        intersections[ei][d] += weight;
      }
    }

    // Now spit out the primitives according to the case and using the
    // already calculated intersection points.
    const int max_nprimitives = (ndims - 1) * (ndims - 1) + 1;
    const int *primitives = NFieldBase<ndims>::CASE_TO_PRIMITIVES[configuration];
    for (int pi = 0;
         pi < max_nprimitives && primitives[pi * ndims] != -1;
         ++pi) {
      for (int d = 0; d < ndims; ++d) {
        position_list.push_back(intersections[primitives[pi * ndims + d]]);
      }
    }
  }

  // Copy list into static buffer.
  nprimitives = position_list.size() / ndims;
  positions = new float[ndims * position_list.size()];
  float *position = positions;
  for (typename std::list<QVector<float, ndims> >::iterator pi = position_list.begin();
       pi != position_list.end();
       ++pi) {
    for (int d = 0; d < ndims; ++d) {
      position[d] = (*pi)[d];
    }
    position += ndims;
  }
}

/* ------------------------------------------------------------------------- */

}

#endif
