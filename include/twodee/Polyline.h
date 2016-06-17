#ifndef POLYLINE_H
#define POLYLINE_H

#include <algorithm>
#include <vector>

#include "Line.h"
#include "Log.h"
#include "NField.h"
#include "Plane.h"
#include "QMatrix4.h"
#include "QVector2.h"
#include "QVector3.h"
#include "Triangle.h"
#include "Trimesh.h"
#include "Vector.h"

using std::vector;

namespace td {

/* ------------------------------------------------------------------------- */

template<class T> class Polyline : public NField<T, 1> {
  public:
    static const bool OPEN = true;
    static const bool CLOSED = false;

    /**
     Creates a new polyline.
     @param nvertices Number of vertices in line
     @param ndims Number of dimensions describing each vertex
     @param is_open Options describing line's status
     */
    Polyline(int nvertices, int ndims, bool is_open);

    /**
     Frees resources consumed by this line.
     */
    ~Polyline();

    Trimesh *Revolve(const QVector3<T>& axis, int nstops, float degrees = 360.0f) const;
    Trimesh *Dowel(int nstops, T radius, bool is_capped = true, T twist = (T) 0, float max_bend = 361.0f) const;
    void Fracture(T max_segment_length);
    bool IsOpen() const;
    QVector3<T> GetNormal() const;

    Polyline<T> *Flatten() const;
    bool IsCounterclockwise() const;
    Trimesh *Triangulate(bool fixWinding = true) const;
    Trimesh *Extrude(const QVector3<T> &axis, T distance, const QMatrix4<float>& xform = QMatrix4<float>(1.0f)) const;

    bool IsPlanar(T threshold) const;

    std::string ToString() const;

  private:
    static QVector3<T> GetConstrainedPerpendicular(const QVector3<T> &axis);

    /** Polyline status */
    bool is_open;
};

/* ------------------------------------------------------------------------- */

template<class T>
Polyline<T>::Polyline(int nvertices, int ndims, bool is_open) :
  NField<T, 1>(QVector<int, 1>(nvertices), ndims),
  is_open(is_open) {
}

/* ------------------------------------------------------------------------- */

template<class T>
Polyline<T>::~Polyline() {
}

/* ------------------------------------------------------------------------- */

template<class T>
Trimesh *Polyline<T>::Revolve(const QVector3<T>& axis, int nstops, float degrees) const {
  assert(nstops > 0);
  assert(degrees <= 360.0f && degrees >= -360.0f);
  assert(fabs(degrees) > 0);

  bool is_full = false;
  if (fabs(degrees - 360.0f) < 1.0e-3f) {
    degrees = 360.0f;
    is_full = true;
  } else if (fabs(degrees - -360.0f) < 1.0e-3f) {
    degrees = -360.0f;
    is_full = true;
  }

  int nmetas = this->GetChannelCount() - 3;

  NField<float, 2> img(QVector2<int>(this->GetElementCount(), nstops), this->GetChannelCount());
  QMatrix4<float> rot;
  if (is_full) {
    rot = QMatrix4<float>::GetRotate(degrees / nstops, axis);
  } else {
    rot = QMatrix4<float>::GetRotate(degrees / (nstops - 1), axis);
  }

  const float EPSILON = 1.0e-3f;

  // find a point that's not the origin, rotate it a bit, subtract off the original
  // that's our relative direction of travel
  QVector3<T> delta;
  for (int i = 0; i < this->GetElementCount(); ++i) {
    QVector3<T> position((*this)(i));
    QMatrix4<float> xform = QMatrix4<float>::GetRotate(degrees < 0 ? -10 : 10, axis);
    QVector3<T> rotated_position = xform * position;
    delta = rotated_position - position;
    if (delta.GetLength() > EPSILON) {
      break;
    }
  }

  // Fill the first row with the polyline coordinates.
  /* QVector3<T> position((T) 0); */
  QVector2<int> i(0, 0);
  for (i[0] = 0; i[0] < this->GetElementCount(); ++i[0]) {
    memcpy(img(i), (*this)(i[0]), sizeof(T) * this->GetChannelCount());
  }

  QVector3<T> c;
  QVector3<T> ct;

  // For each remaining row, rotate the coordinates in the row below.
  for (int ri = 1; ri < nstops; ++ri) {
    for (i[0] = 0; i[0] < this->nelements; ++i[0]) {
      i[1] = ri - 1;
      c[0] = img(i)[0];
      c[1] = img(i)[1];
      c[2] = img(i)[2];
      ct = rot * c;

      T *vertex_meta = img(i) + 3;

      i[1] = ri;
      img(i)[0] = ct[0];
      img(i)[1] = ct[1];
      img(i)[2] = ct[2];

      memcpy(img(i) + 3, vertex_meta, sizeof(T) * nmetas);
    }
  }

  Trimesh *base = Trimesh::GetParametric(img, !is_open, is_full);

  if (this->GetElementCount() >= 3) {
    Polyline<T> *flattened = this->Flatten();
    bool is_ccw = flattened->IsCounterclockwise();
    delete flattened;

    QVector3<T> normal = this->GetNormal();
    T normal_dot_direction = normal.Dot(delta);

    if ((is_ccw && normal_dot_direction < 0) ||
        (!is_ccw && normal_dot_direction < 0)) {
      base->ReverseWinding();
    }

    // Add caps if this isn't a full revolution but the cross section is planar.
    if (this->IsPlanar(1.0e-6f) && !is_full) {
      // The first cap is easy. Just take our framing polyline and
      // triangulate it.
      Trimesh *cap_a_mesh = this->Triangulate(); 

      if (nmetas > 0) {
        T *vertex_meta = cap_a_mesh->AllocateVertexMetas(nmetas);
        for (int i = 0; i < this->GetElementCount(); ++i) {
          memcpy(vertex_meta, (*this)(i) + 3, sizeof(T) * nmetas);
          vertex_meta += nmetas;
        }
      }

      if ((is_ccw && normal_dot_direction > 0) ||
          (!is_ccw && normal_dot_direction < 0)) {
        cap_a_mesh->ReverseWinding();
      }

      QMatrix4<float> xform = QMatrix4<float>::GetRotate(degrees, axis);
      Trimesh cap_b_mesh(*cap_a_mesh);
      cap_b_mesh *= xform;
      cap_b_mesh.ReverseWinding();

      *base += *cap_a_mesh;
      *base += cap_b_mesh;

      delete cap_a_mesh;
    }
  }

  base->CoalesceVertices(3);

  return base;
}

/* ------------------------------------------------------------------------- */

template<class T>
QVector3<T> Polyline<T>::GetConstrainedPerpendicular(const QVector3<T> &axis) {
  // We need to push out from the first node to its surrounding ring. Let's
  // find a vector that leads us out.
  QVector3<T> to_ring;

  // We can really shoot out anywhere in this node's plane, but if this leg of
  // the polyline is 2D and axis-aligned, let's shoot out in its 2D plane. We
  // want a vector that is perpendicular to the normal and does not point out
  // at all along the omitted third dimension. We start by seeing if normals[0]
  // has a 0 somewhere. If so, we're 2D.
  int dzero = -1;
  for (int i = 0; i < 3 && dzero == -1; ++i) {
    if (fabs(axis[i]) < 1.0e-6f) {
      dzero = i;
    }
  }

  // If we found no zeroed dimension, any old perpendicular vector will do.
  if (dzero < 0) {
    to_ring = axis.GetPerpendicular();
  }

  // Otherwise, we create a vector that's perpendicular but is constrained
  // to the 2D plane.
  else {

    // We create a new normal where the zeroed dimension is omitted.
    QVector2<T> projected_axis;
    for (int d = 0, i = 0; d < 3; ++d) {
      if (d != dzero) {
        projected_axis[i] = axis[d];
        ++i;
      }
    }

    // Then we find vector perpendicular to this one. There are only two
    // choices now since we've lost a dimension.
    QVector2<T> projected_to_ring = projected_axis.GetPerpendicular();

    // Now we expanded this 2D vector back into 3D, keeping the missing
    // dimension at 0 and staying within the plane.
    for (int d = 0, i = 0; d < 3; ++d) {
      if (d != dzero) {
        to_ring[d] = projected_to_ring[i];
        ++i;
      } else {
        to_ring[d] = 0.0f;
      }
    }
  }

  return to_ring;
}

/* ------------------------------------------------------------------------- */

template<typename T>
struct VertexMeta {
  QVector3<T> position;
  const T *meta;

  VertexMeta(const QVector3<T> &position,
              const T *meta) :
    position(position),
    meta(meta) {
  }
};

/* ------------------------------------------------------------------------- */

template<class T>
Trimesh *Polyline<T>::Dowel(int nstops, T radius, bool is_capped, T twist, float max_bend) const {
  assert(max_bend > 0.0f);

  int nvertices = this->GetElementCount();
  int ndims = this->GetChannelCount();
  bool is_adaptive_radius = radius < 0.0f;

  assert(nstops > 2);
  assert(nvertices > 1);

  // Assert elements have at least XYZ in channels 012. If radius is in there
  // too, we'll need that in channel 3.
  assert(ndims >= 3);
  assert(!is_adaptive_radius || ndims >= 4);

  const float EPSILON = 1.0e-3f;

  // Get the vertex positions in a more usable vector form.
  QVector3<T> *nodes = new QVector3<T>[nvertices]; 
  for (int vi = 0; vi < nvertices; ++vi) {
    nodes[vi] = QVector3<T>((*this)(vi));
  }

  // We'll need forward and backward vectors for all the nodes. In general,
  // fore points to the next vertex, and aft points to the previous vertex.
  QVector3<T> *afts = new QVector3<T>[nvertices]; 
  QVector3<T> *fores = new QVector3<T>[nvertices]; 

  // If this polyline is open, then aft is ill-defined on the first node and
  // fore is ill-defined on the last. We give the null vector for these.
  if (is_open) {
    afts[0] = QVector3<T>((T) 0);
    fores[nvertices - 1] = QVector3<T>((T) 0);
  }

  // We also mark the index bounds of which vertices are "internal."
  int first_successor = is_open ? 1 : 0;
  int last_predecessor = is_open ? nvertices - 2 : nvertices - 1;

  // For the internal nodes, we compute fores and afts by finding the
  // difference vectors from each vertex to its neighbors.
  for (int vi = 0; vi <= last_predecessor; ++vi) {
    fores[vi] = nodes[(vi + 1) % nvertices] - nodes[vi];
    if (fores[vi].GetLength() > EPSILON) {
      fores[vi].Normalize();
    }
  }

  for (int vi = first_successor; vi < nvertices; ++vi) {
    afts[vi] = nodes[(vi - 1 + nvertices) % nvertices] - nodes[vi];
    if (afts[vi].GetLength() > EPSILON) {
      afts[vi].Normalize();
    }
  }

  // Any node that spatially coincides with its predecessor will have a bad
  // aft, i.e., its aft will be the null vector. So, we share a predecessor's
  // non-degenerate aft with its coinciding successor. If multiple nodes all
  // sit atop each other, the first good aft will propogate along.
  for (int vi = 1; vi < nvertices; ++vi) {
    if (afts[vi].GetLength() < EPSILON) {
      afts[vi] = afts[vi - 1];
    }
  }

  // When previous has a move 0, afts[0] is bad.
  if (!is_open) {
    for (int vi = 0; vi < nvertices && afts[vi].GetLength() < EPSILON; ++vi) {
      afts[vi] = afts[(nvertices + vi - 1) % nvertices];
    }
  }

  // Any node that coincides with its successor will have a bad fore. See
  // above. We must work backwards to propogate in the right direction.
  for (int vi = nvertices - 2; vi >= 0; --vi) {
    if (fores[vi].GetLength() < EPSILON) {
      fores[vi] = fores[vi + 1];
    }
  }

  if (!is_open) {
    for (int vi = nvertices - 2; vi >= 0 && fores[vi].GetLength() < EPSILON; --vi) {
      fores[vi] = fores[vi + 1];
    }
  }

  // At each node in the line, we'll impose a plane that slices through it.
  // For internal nodes, this plane bisects the angle formed by this node and
  // its neighbors. We'll send along the solid rays from the predecessor's ring
  // to intersect these planes. This will allow us to maintain some coherence
  // along the solid dowel. Instead of generating afresh a ring around each
  // node, we generate the ring once at the initiating node and trace out the
  // ring's polylines along the overall polyline.
  //
  // We calculate the planes' normals according to their nodes' connectedness
  // to its neighbors. In general, the plane will be oriented to bisect the
  // angle formed by three nodes.
  QVector3<T> *normals = new QVector3<T>[nvertices]; 
  for (int vi = 0; vi < nvertices; ++vi) {
    if (fores[vi].GetSquaredLength() < EPSILON && afts[vi].GetSquaredLength() < EPSILON) {
      throw MessagedException("I can't solidify this dowel. Too many of its vertices are piled on top of each other.");
    } else if (fores[vi].GetSquaredLength() < EPSILON) {
      normals[vi] = afts[vi] * (T) -1;
    } else if (afts[vi].GetSquaredLength() < EPSILON) {
      normals[vi] = fores[vi];
    } else if (fabs(fores[vi].Dot(afts[vi])) > 0.999999f) {
      normals[vi] = afts[vi] * (T) -1;
    } else {
      QVector3<T> tangent = fores[vi] + afts[vi];
      QVector3<T> bitangent = fores[vi].Cross(afts[vi]);
      normals[vi] = bitangent.Cross(tangent);
      normals[vi].Normalize();
      if (vi == 0) {
        normals[vi] *= (T) -1;
      }
    }
  }
 
  vector<VertexMeta<T> > cap_neighbors[2];
  vector<VertexMeta<T> > vertices;
  Plane<T, 3> first_plane(nodes[0], normals[0]);

  // Now generate geometry for the rest of the vertices.
  for (int vi = 0; vi < nvertices; ++vi) {
    // Bend is measured as the angle at interior contact point of the join. At
    // the node itself, angle is acos(fore.dot(aft)). If we walk from the node
    // the same distance along fore and aft till we make right angles, and then
    // walk perpendicular lines out to the internal contact point, we'll have a
    // quadrilateral. The angles of the quad sum to 360, and with the angle at
    // the node known and the other two being 90, we can solve for the angle at
    // the contact point.
    T radians_at_node = acos(fores[vi].Dot(afts[vi]));
    T bend_degrees = 360.0f - 180.0f - radians_at_node * 180 / td::PI;

    if (is_adaptive_radius) {
      radius = (*this)(vi)[3];
    }

    const T *meta = &(*this)(vi)[3];

    // See if we need to round this bend. We round if two things are true: 1)
    // the bend exceeds the maximum allowed, and 2) we're not an endpoint node
    // or we are an endpoint but the line is closed.
    if (((vi != nvertices - 1 && vi != 0) || !is_open) &&
        bend_degrees > max_bend) {

      // Find a vector that bisects the angle between aft and fore.
      QVector3<T> bisector = fores[vi] + afts[vi];
      bisector.Normalize();

      // Push out from node along bisector. Go out far enough so that the aft
      // and fore shafts just meet and do not overlap. This point will be our
      // pivot. We determine "far enough" by imposing a right triangle onto aft
      // shaft. The 90-degree angle sits at the shaft and leads on to the node
      // and out to the contact point. The hypotenuse is the distance from the
      // node to the contact point.
      QVector3<T> pivot = nodes[vi] + bisector * radius / sinf(radians_at_node * 0.5f);

      // Project pivot onto aft. This will be the first pseudonode of the
      // rounded joint.
      QVector3<T> pivot_on_aft = nodes[vi] + afts[vi] * afts[vi].Dot(pivot - nodes[vi]) / afts[vi].Dot(afts[vi]);

      // Find an axis about which we can rotate.
      QVector3<T> pivot_axis = fores[vi].Cross(afts[vi]);
      pivot_axis.Normalize();

      // What's the plane of the first slice?
      Plane<T, 3> plane(pivot_on_aft, afts[vi]);
 
      // Generate ring on first slice. We'll do so by sending lines from the
      // previous vertices (if any) along the aft vector and intersecting them
      // with this slice's plane.
      if (vi > 0) {
        int i_previous_ring = vertices.size() - nstops;
        for (int si = 0; si < nstops; ++si) {
          QVector3<T> previous_ring_neighbor(vertices[i_previous_ring + si].position);
          Line<T, 3> line(previous_ring_neighbor, afts[vi] * (T) -1);

          QVector3<T> intersection_point;
          plane.Intersect(line, intersection_point);
          if (is_adaptive_radius) {
            QVector3<T> diff = intersection_point - pivot_on_aft;
            diff.Normalize();
            intersection_point = pivot_on_aft + diff * radius;
          }
          vertices.push_back(VertexMeta<T>(intersection_point, meta));
        }
      }

      // But what if we haven't generated any vertices before? Let's do so now.
      // We do similar to how we would have done so at a non-rounded node, but
      // we use a plane perpendicular to aft instead of one that bisects the
      // angle at the node.
      else {
        first_plane = plane;

        QVector3<T> to_ring = GetConstrainedPerpendicular(afts[0]);
        to_ring *= radius;
        to_ring = QMatrix4<float>::GetRotate(twist, afts[0]) * to_ring;
        for (int si = 0; si < nstops; ++si) {
          QVector3<T> ring_neighbor = QMatrix4<float>::GetRotate(si * -360.0f / nstops, afts[0]) * to_ring + pivot_on_aft;
          vertices.push_back(VertexMeta<T>(ring_neighbor, meta));
        }
      }

      // With the first slice emitted, let's figure out how many more we need
      // and how much we'll have to rotate between each so as to not exceed the
      // maximum bend. We divvy the bend up into chunks that are no larger than
      // max_bend.
      int nslices = ceilf(bend_degrees / max_bend);
      T bend_delta = bend_degrees / (nslices - 1);
      const QMatrix4<T> rotator = QMatrix4<T>::GetTranslate(pivot[0], pivot[1], pivot[2]) *
                                  QMatrix4<T>::GetRotate(bend_delta, pivot_axis) *
                                  QMatrix4<T>::GetTranslate(-pivot[0], -pivot[1], -pivot[2]);

      // Generate remaining slices by rotating the previous slice's vertices.
      for (int i = 1; i < nslices; ++i) {
        int i_previous_ring = vertices.size() - nstops;
        for (int si = 0; si < nstops; ++si) {
          QVector3<T> previous_ring_neighbor(vertices[i_previous_ring + si].position);
          QVector3<T> ring_neighbor = rotator * previous_ring_neighbor;
          vertices.push_back(VertexMeta<T>(ring_neighbor, meta));
        }
      }
    }

    // If this polyline is disconnected or the first node isn't sharp enough to
    // warrant rounding, let's emit our first set of vertices around the first
    // node.
    else if (vi == 0) {
      // Follow node out to surrounding ring according to the current radius. We
      // choose a vector to stay 2D axis-aligned if possible.
      QVector3<T> to_ring = GetConstrainedPerpendicular(fores[0]);
      to_ring *= radius;
   
      // We let the caller apply a twist to the seed vector to perfect alignment.
      to_ring = QMatrix4<float>::GetRotate(twist, fores[0]) * to_ring;

      // Spin that vector around the desired number of stops and emit a vertex.
      for (int si = 0; si < nstops; ++si) {
        QVector3<T> ring_neighbor = QMatrix4<float>::GetRotate(si * 360.0f / nstops, fores[0]) * to_ring + nodes[0];
        Line<T, 3> line(ring_neighbor, fores[0] * (T) -1);

        QVector3<T> intersection_point;
        first_plane.Intersect(line, intersection_point);
        vertices.push_back(VertexMeta<T>(intersection_point, meta));

        // If this line is disconnected, let's record this vertex as also being
        // part of the cap.
        if (is_open) {
          cap_neighbors[0].push_back(VertexMeta<T>(intersection_point, meta));
        }
      }
    }

    else if (nodes[vi].GetDistanceTo(nodes[vi - 1]) < EPSILON) {
      int i_previous_ring = vertices.size() - nstops;
      for (int si = 0; si < nstops; ++si) {
        QVector3<T> previous_ring_neighbor(vertices[i_previous_ring + si].position);
        QVector3<T> diff = previous_ring_neighbor - nodes[vi];
        diff.Normalize();
        QVector3<T> scaled = nodes[vi] + diff * radius;
        vertices.push_back(VertexMeta<T>(scaled, meta));
 
        // If this is the last node on an open line, let's capture these
        // vertices for the cap as well.
        if (vi == this->GetElementCount() - 1 && is_open) {
          cap_neighbors[1].push_back(VertexMeta<T>(scaled, meta));
        }
      }
    }

    // Okay, so this node's joint doesn't need rounding. Let's take the
    // previous vertices and shoot a line from them along aft into this node's
    // bisecting plane. Those intersection points are this node's ring of
    // neighbors.
    else {
      Plane<T, 3> plane(nodes[vi], normals[vi]);
      Plane<T, 3> aft_plane(nodes[vi], afts[vi]);

      int i_previous_ring = vertices.size() - nstops;
      for (int si = 0; si < nstops; ++si) {
        QVector3<T> previous_ring_neighbor(vertices[i_previous_ring + si].position);
        Line<T, 3> line(previous_ring_neighbor, afts[vi] * (T) -1);

        QVector3<T> intersection_point;
        aft_plane.Intersect(line, intersection_point);
        if (is_adaptive_radius) {
          QVector3<T> diff = intersection_point - aft_plane.GetPoint();
          diff.Normalize();
          intersection_point = nodes[vi] + diff * radius;
        }

        line = Line<T, 3>(intersection_point, afts[vi] * (T) -1);
        plane.Intersect(line, intersection_point);

        vertices.push_back(VertexMeta<T>(intersection_point, meta));

        // If this is the last node on an open line, let's capture these
        // vertices for the cap as well.
        if (vi == this->GetElementCount() - 1 && is_open) {
          cap_neighbors[1].push_back(VertexMeta<T>(intersection_point, meta));
        }
      }
    }
  }

  // If the line is closed, we need the fill in geometry between the last node
  // and the first. Trimesh::GetParametric can do this for us, but the model
  // may have gotten twisted and what was the ith neighbor at the first node
  // might not align at the end. So, we continue our previous trick of shooting
  // the last vertices into the first node's plane. The duplicate vertices will
  // need to be cleaned up later.
  if (!is_open) {
    Plane<T, 3> aft_plane(nodes[0], afts[0]);
    int i_previous_ring = vertices.size() - nstops;
    for (int si = 0; si < nstops; ++si) {
      QVector3<T> previous_ring_neighbor(vertices[i_previous_ring + si].position);
      Line<T, 3> line(previous_ring_neighbor, fores[nvertices - 1] * (T) -1);

      QVector3<T> intersection_point;
      aft_plane.Intersect(line, intersection_point);

      line = Line<T, 3>(intersection_point, fores[nvertices - 1] * (T) -1);
      first_plane.Intersect(line, intersection_point);

      if (is_adaptive_radius) {
        QVector3<T> diff = intersection_point - first_plane.GetPoint();
        diff.Normalize();
        intersection_point = first_plane.GetPoint() + diff * (*this)(0)[3];
      }

      vertices.push_back(VertexMeta<T>(intersection_point, &(*this)(0)[3]));
    }
  }

  // The vertices have all been emitted. We're done with the bookkeeping.
  delete[] nodes;
  delete[] normals;
  delete[] afts;
  delete[] fores;

  int nmetas = this->GetChannelCount() - 3;

  // We lay out the vertices in an nstops x ~nvertices grid. At each cell is a
  // vertex's 3D position.
  NField<float, 2> img(QVector2<int>(nstops, vertices.size() / nstops), this->GetChannelCount());
  NFieldIterator<2> c(img.GetDimensions());
  int i = 0;
  while (c.HasNext()) {
    c.Next();
    memcpy(img(c), &vertices[i].position[0], sizeof(T) * 3);
    memcpy(&img(c)[3], vertices[i].meta, sizeof(T) * nmetas);
    ++i;
  }

  // We expand the parameteric surface into a full triangular mesh. The columns
  // wrap (following the solid gerth of tube), but not the rows. We wrapped
  // those manually as needed.
  Trimesh *mesh = Trimesh::GetParametric(img, true, false);
  mesh->MigrateVertexMetasToColors(1, 2, 3);

  // If this mesh is open, we need to generate caps as well.
  if (is_open && is_capped) {
    for (int ci = 0; ci < 2; ++ci) {
      int vi = ci == 0 ? 0 : (this->GetElementCount() - 1);
      Trimesh *cap = new Trimesh(cap_neighbors[ci].size() + 1, nstops);

      float *position = cap->GetPositions();
      float *meta = cap->AllocateVertexMetas(nmetas);

      // Center vertex
      memcpy(position, (*this)(vi), sizeof(T) * 3);
      memcpy(meta, &(*this)(vi)[3], sizeof(T) * nmetas);
      position += 3;
      meta += nmetas;

      for (unsigned int i = 0; i < cap_neighbors[ci].size(); ++i) {
        memcpy(position, &cap_neighbors[ci][i].position[0], sizeof(T) * 3);
        memcpy(meta, cap_neighbors[ci][i].meta, sizeof(T) * nmetas);
        position += 3;
        meta += nmetas;
      }

      int *face = cap->GetFaces();
      for (unsigned int i = 0; i < cap_neighbors[ci].size(); ++i) {
        face[0] = 0;
        face[1] = i + 1;
        face[2] = (i + 1) % cap_neighbors[ci].size() + 1;
        if (ci == 0) {
          int tmp = face[1];
          face[1] = face[2];
          face[2] = tmp;
        }
        face += 3;
      }

      *mesh += *cap;
      delete cap;
    }
  }

  /* mesh->WriteJSON("boo.json"); */

  return mesh;
}

/* ------------------------------------------------------------------------- */

template<class T>
void Polyline<T>::Fracture(T max_segment_length) {
  int nvertices = this->GetElementCount();
  int ndims = this->GetChannelCount();

  // Get the vertex positions in a more usable vector form.
  Vector<T> *old_vertices = new Vector<T>[nvertices]; 
  for (int vi = 0; vi < nvertices; ++vi) {
    old_vertices[vi] = Vector<T>(ndims, (*this)(vi));
  }

  Vector<T> *diffs = new Vector<T>[nvertices];

  int new_nvertices = nvertices;
  int up_to = is_open ? nvertices - 1 : nvertices;
  for (int vi = 0; vi < up_to; ++vi) {
    // Find the vector from this element to the next -- and its length.
    diffs[vi] = old_vertices[(vi + 1) % nvertices] - old_vertices[vi];
    T segment_length = diffs[vi].GetLength();

    // If the vector is longer than allowed, let's break this segment up.
    if (segment_length > max_segment_length) {
      int nsegments = (int) ceil(segment_length / max_segment_length);
      new_nvertices += nsegments - 1;
    }
  }

  // Only if there's a change in vertex count do we bother.
  if (new_nvertices != nvertices) {
    this->Resize(QVector<int, 1>(new_nvertices), ndims);
    int new_ei = 0;

    // For each element but last, copy it over and break up its segment if necessary.
    for (int vi = 0; vi < up_to; ++vi) {
      memcpy((*this)(new_ei), &old_vertices[vi][0], sizeof(T) * ndims);
      ++new_ei;
 
      // If the vector is longer than allowed, let's break this segment up.
      T segment_length = diffs[vi].GetLength();
      if (segment_length > max_segment_length) {
        int nsegments = (int) ceil(segment_length / max_segment_length);
        T fracture_length = segment_length / nsegments;
        diffs[vi].Normalize();
        diffs[vi] *= fracture_length;
        for (int fi = 0; fi < nsegments - 1; ++fi) {
          old_vertices[vi] += diffs[vi];
          memcpy((*this)(new_ei), &old_vertices[vi][0], sizeof(T) * ndims);
          ++new_ei;
        }
      }
    }

    // If we're closed, the loop above will have already inserted the last vertex
    // and the fracture points after. If we're open, we still need to insert that
    // last element. This is the half of the loop-and-a-half.
    if (is_open) {
      memcpy((*this)(new_ei), &old_vertices[nvertices - 1][0], sizeof(T) * ndims);
    }
  } 

  delete[] old_vertices;
  delete[] diffs;
}

/* ------------------------------------------------------------------------- */

template<class T>
std::string Polyline<T>::ToString() const {
  std::stringstream ss;
  ss << "[";
  for (int i = 0; i < this->GetElementCount(); ++i) {
    ss << "[";
    ss << (*this)(i)[0];
    for (int ci = 1; ci < this->GetChannelCount(); ++ci) {
      ss << ", " << (*this)(i)[ci];
    }
    ss << "]" << (i == this->GetElementCount() - 1 ? "" : ", ");
  }
  ss << "]";
  return ss.str();
}

/* ------------------------------------------------------------------------- */

template<typename T>
struct TriangulateVertex {
  QVector3<T> vertex3;
  QVector2<T> vertex2;
  int array_index;

  TriangulateVertex(const QVector3<T> vertex3,
                    const QVector2<T> vertex2,
                    int array_index) :
    vertex3(vertex3),
    vertex2(vertex2),
    array_index(array_index) {
  }
};

/* ------------------------------------------------------------------------- */

template<class T>
bool Polyline<T>::IsCounterclockwise() const {
  int nvertices = this->GetElementCount();
  int ndims = this->GetChannelCount();
  assert(ndims == 2);
  assert(nvertices >= 3);

  T signed_area = 0;
  for (int i = 0; i < nvertices; ++i) {
    QVector2<T> a((*this)(i));
    QVector2<T> b((*this)((i + 1) % nvertices));
    signed_area += (b[0] - a[0]) * (b[1] + a[1]);
  }
  
  return signed_area < 0;
}

/* ------------------------------------------------------------------------- */

template<class T>
QVector3<T> Polyline<T>::GetNormal() const {
  int nvertices = this->GetElementCount();
  int ndims = this->GetChannelCount();
  assert(ndims >= 3);

  QVector3<T> normal((T) 0); 
  for (int i = 0; i < nvertices; ++i) {
    QVector3<T> a((*this)(i));
    QVector3<T> b((*this)((i + 1) % nvertices));
    normal += a.Cross(b);
  }

  normal.Normalize();
  return normal;
}

/* ------------------------------------------------------------------------- */

template<class T>
Polyline<T> *Polyline<T>::Flatten() const {
  int nvertices = this->GetElementCount();
  int ndims = this->GetChannelCount();
  assert(ndims >= 3);

  // We project the triangle down to two dimensions. A simple way to do this is
  // to get the polygon's normal and drop the dimension of the component with
  // the largest magnitude.
  QVector3<T> a((*this)(0));
  QVector3<T> b((*this)(1));
  QVector3<T> c((*this)(2));

  QVector3<T> fro = a - b;
  QVector3<T> to = c - b;
  QVector3<T> normal_abs = fro.Cross(to).Abs();

  QVector2<int> d;
  if (normal_abs[0] > normal_abs[1] && normal_abs[0] > normal_abs[2]) {
    d[0] = 1;
    d[1] = 2;
  } else if (normal_abs[1] > normal_abs[0] && normal_abs[1] > normal_abs[2]) {
    d[0] = 0;
    d[1] = 2;
  } else {
    d[0] = 0;
    d[1] = 1;
  }
  
  Polyline<T> *flattened = new Polyline<T>(this->GetElementCount(), 2, is_open);
  for (int i = 0; i < nvertices; ++i) {
    (*flattened)(i)[0] = (*this)(i)[d[0]];
    (*flattened)(i)[1] = (*this)(i)[d[1]];
  }

  return flattened;
}

/* ------------------------------------------------------------------------- */

template<class T>
Trimesh *Polyline<T>::Triangulate(bool fixWinding) const {
  // Assumes polyline traces planar polygon.

  // Solving this problem is a lot easier in two dimensions. We project the
  // triangle down to two dimensions. A simple way to do this is to get the
  // polygon's normal and drop the dimension of the component with the largest
  // magnitude.
  Polyline<T> *flattened = this->Flatten();
  
  const int nvertices = this->GetElementCount();
  vector<QVector3<T> > positions;
  vector<QVector3<int> > faces;
  vector<TriangulateVertex<T> > remaining;
  for (int i = 0; i < nvertices; ++i) {
    QVector3<float> position(QVector3<float>((*this)(i)));
    positions.push_back(position);
    remaining.push_back(TriangulateVertex<T>(position, QVector2<T>((*flattened)(i)), i));
  }

  // A negative signed area means the vertices are enumerated in clockwise
  // order in the Cartesian coordinate system with the origin at (0, 0) and
  // they y-axis pointing up.
  bool is_reversed = false;
  if (!flattened->IsCounterclockwise()) {
    std::reverse(remaining.begin(), remaining.end());
    is_reversed = true;
  }

  // While we have at least three vertices left, find an ear and make a face of it.
  while (remaining.size() > 2) {
    // Look for an ear starting at each vertex in the list.
    bool is_ear_found = false;
    for (size_t i = 0; !is_ear_found && i < remaining.size(); ++i) {
      // Wrap around as needed.
      size_t j = (i + 1) % remaining.size();
      size_t k = (i + 2) % remaining.size();

      // See if this angle's interior angle is < 180 degrees. If it is, we can't make
      // a triangle here. It would fill an area outside the polygon.
      QVector2<T> fro = remaining[i].vertex2 - remaining[j].vertex2;
      QVector2<T> to = remaining[k].vertex2 - remaining[j].vertex2;

      float signed_area = to[0] * fro[1] - to[1] * fro[0];
      if (signed_area < 1.0e-6f) {
        if (i == remaining.size() - 1) {
          throw MessagedException("no good angle");
        } else {
          continue;
        }
      }

      // See if any other vertex lies inside the triangle formed by ijk.
      bool contains_vertex = false;
      for (size_t ci = 0; !contains_vertex && ci < remaining.size(); ++ci) {
        if (ci != i && ci != j && ci != k) {
          Triangle2<T> tri(remaining[i].vertex2, remaining[j].vertex2, remaining[k].vertex2);
          contains_vertex = tri.Contains(remaining[ci].vertex2);
        }
      }

      // If no vertex is inside the triangle formed by ijk, it's an ear. Let's
      // lop it off by removing the central vertex from the polygon. The
      // vertices form a face.
      if (!contains_vertex || remaining.size() == 3) {
        faces.push_back(QVector3<int>(remaining[i].array_index, remaining[j].array_index, remaining[k].array_index));
        remaining.erase(remaining.begin() + j);
        is_ear_found = true;
      }
    }

    if (!is_ear_found) {
      throw MessagedException("no ear");
    }
  }

  Trimesh *mesh = new Trimesh(positions, faces);;
  if (!fixWinding && is_reversed) {
    mesh->ReverseWinding();
  }

  return mesh;
}

/* ------------------------------------------------------------------------- */

template<class T>
Trimesh *Polyline<T>::Extrude(const QVector3<T> &axis, T distance, const QMatrix4<float>& xform) const {
  // assumes axis is normalized
  int nvertices = this->GetElementCount();
  bool is_planar = this->IsPlanar(1.0e-6f);

  // Two directions...
  const QVector3<T> delta = axis * distance;
  QVector3<T> normal = this->GetNormal();

  // How does this compare with where we are headed?
  float normal_dot_direction = normal.Dot(delta);

  // Which way is end A wound?
  Polyline<T> *flattened = this->Flatten();
  bool is_ccw = flattened->IsCounterclockwise();
  delete flattened;

  vector<VertexMeta<T> > vertices; 
  vector<QVector3<int> > faces; 

  int nmetas = this->GetChannelCount() - 3;

  // End A
  Trimesh *cap_a = is_planar ? this->Triangulate() : new Trimesh(0, 0); 
  float *vertex_meta = NULL;
  if (nmetas > 0) {
    vertex_meta = cap_a->AllocateVertexMetas(nmetas);
  }

  for (int i = 0; i < nvertices; ++i) {
    vertices.push_back(VertexMeta<T>(QVector3<T>((*this)(i)), &(*this)(i)[3]));
    if (is_planar && vertex_meta) {
      memcpy(vertex_meta, &(*this)(i)[3], sizeof(float) * nmetas);
      vertex_meta += nmetas;
    }
  }

  // End B
  for (int i = 0; i < nvertices; ++i) {
    vertices.push_back(VertexMeta<T>(xform * (vertices[i].position + delta), vertices[i].meta));
  }

  for (int i = 0; i < nvertices; ++i) {
    faces.push_back(QVector3<int>(i, (i + nvertices - 1) % nvertices + nvertices, (i + nvertices - 1) % nvertices));
    faces.push_back(QVector3<int>(i, i + nvertices, (i + nvertices - 1) % nvertices + nvertices));
  }

  vector<QVector3<T> > positions;
  for (unsigned int i = 0; i < vertices.size(); ++i) {
    positions.push_back(vertices[i].position);
  }
  Trimesh *extruded = new Trimesh(positions, faces);

  if (nmetas > 0) {
    float *vertex_meta = extruded->AllocateVertexMetas(nmetas);
    for (unsigned int i = 0; i < vertices.size(); ++i) {
      memcpy(vertex_meta, vertices[i].meta, sizeof(float) * nmetas);
      vertex_meta += nmetas;
    }
  }

  if (!is_ccw && normal_dot_direction < 0) {
    extruded->ReverseWinding();
    cap_a->ReverseWinding();
  } else if (is_ccw && normal_dot_direction > 0) {
    cap_a->ReverseWinding();
  } else if (is_ccw && normal_dot_direction < 0) {
    extruded->ReverseWinding();
  }

  *extruded += *cap_a;
  Trimesh *cap_b = new Trimesh(*cap_a);
  *cap_b *= xform;
  cap_b->ReverseWinding();
  *cap_b += delta;
  *extruded += *cap_b;

  delete cap_a;
  delete cap_b;

  return extruded;
}

/* ------------------------------------------------------------------------- */

template<class T>
bool Polyline<T>::IsOpen() const {
  return is_open;
}

/* ------------------------------------------------------------------------- */

template<class T>
bool Polyline<T>::IsPlanar(T threshold) const {
  if (this->GetChannelCount() < 3) {
    return true;
  }

  // First compute plane determined by first three vertices. 
  QVector3<T> a((*this)(0));
  QVector3<T> b((*this)(1));
  QVector3<T> c((*this)(2));

  QVector3<T> a2b = b - a;
  QVector3<T> a2c = c - a;

  QVector3<T> normal = a2b.Cross(a2c);
  normal.Normalize();

  Plane<T, 3> plane(a, normal);

  // Next make sure each remaining vertex is on that plane.
  for (int i = 3; i < this->GetElementCount(); ++i) {
    T distance = plane.DistanceTo(QVector3<T>((*this)(i)));
    if (fabs(distance) > threshold) {
      return false;
    }
  }

  return true;
}

/* ------------------------------------------------------------------------- */

}

#endif
