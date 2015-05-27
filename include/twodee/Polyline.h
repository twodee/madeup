#ifndef POLYLINE_H
#define POLYLINE_H

#include <vector>

#include "Line.h"
#include "Log.h"
#include "MinHeap.h"
#include "NField.h"
#include "Plane.h"
#include "QMatrix4.h"
#include "QVector2.h"
#include "QVector3.h"
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
    Trimesh *Dowel(int nstops, T radius, T twist = (T) 0) const;
    void Fracture(T max_segment_length);
    bool IsOpen() const;

    Trimesh *Triangulate() const;
    Trimesh *Extrude(const QVector3<T> axis, T distance, const QMatrix4<float>& xform = QMatrix4<float>(1.0f)) const;

    std::string ToString() const;

  private:
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

  if (fabs(degrees - 360.0f) < 1.0e-3f) {
    degrees = 360.0f;
  }
  bool is_full = degrees == 360.0f;

  NField<float, 2> img(QVector2<int>(this->GetElementCount(), nstops), 3);
  QMatrix4<float> rot;
  if (is_full) {
    rot = QMatrix4<float>::GetRotate(-degrees / nstops, axis);
  } else {
    rot = QMatrix4<float>::GetRotate(-degrees / (nstops - 1), axis);
  }

  // Fill the first row with the polyline coordinates.
  QVector3<T> position((T) 0);
  QVector2<int> i(0, 0);
  for (i[0] = 0; i[0] < this->GetElementCount(); ++i[0]) {
    const T *element = (*this)(i[0]);
    for (int d = 0; d < this->GetChannelCount(); ++d) {
      position[d] = element[d];
    }
    memcpy(img(i), &position[0], sizeof(T) * 3);
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

      i[1] = ri;
      img(i)[0] = ct[0];
      img(i)[1] = ct[1];
      img(i)[2] = ct[2];
    }
  }

  Trimesh *base = Trimesh::GetParametric(img, !is_open, degrees == 360.0f);

  // Add caps if this isn't a full revolution.
  if (!is_full) {
    // The first cap is easy. Just take our framing polyline and
    // triangulate it.
    Trimesh *cap_a_mesh = this->Triangulate(); 
    cap_a_mesh->ReverseWinding();
    *base += *cap_a_mesh;
    delete cap_a_mesh;

    // The second cap spans the final rotated framing polyline. We've already
    // calculated that rotated frame. It's in the last row of our field.
    Polyline<T> cap_b_frame(this->GetElementCount(), this->GetChannelCount(), IsOpen());
    for (int i = 0; i < this->GetElementCount(); ++i) {
      QVector2<T> c(this->GetElementCount() - 1 - i, nstops - 1);
      memcpy(cap_b_frame(i), img(c), 3 * sizeof(float));
    }
    Trimesh *cap_b_mesh = cap_b_frame.Triangulate();
    cap_b_mesh->ReverseWinding();
    *base += *cap_b_mesh;
    delete cap_b_mesh;
  }

  return base;
}

/* ------------------------------------------------------------------------- */

template<class T>
Trimesh *Polyline<T>::Dowel(int nstops, T radius, T twist) const {
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

  // If this polyline is open, then fore and aft are ill-defined on the
  // extrema. We give the null vector for these.
  if (is_open) {
    afts[0] = QVector3<T>((T) 0);
    fores[nvertices - 1] = QVector3<T>((T) 0);
  }

  // Otherwise, we can compute fores and afts by finding the difference
  // vectors from each vertex to its neighbors.
  int first_successor = is_open ? 1 : 0;
  int last_predecessor = is_open ? nvertices - 2 : nvertices - 1;

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

  // Any node that coincides with its predecessor will have a bad aft. Share a
  // predecessor's aft with its coinciding successor.
  for (int vi = 1; vi < nvertices; ++vi) {
    if (afts[vi].GetLength() < EPSILON) {
      afts[vi] = afts[vi - 1];
    }
  }

  // Any node that coincides with its successor will have a bad fore. Share a
  // successor's aft with its coinciding predecessor.
  for (int vi = nvertices - 2; vi >= 0; --vi) {
    if (fores[vi].GetLength() < EPSILON) {
      fores[vi] = fores[vi + 1];
    }
  }

  // With fores and afts for each vertex, we can compute a vector that points
  // away from each bend in the dowel. We want solid normals to make the
  // geometry align, so we do some finessing here. If fore or aft are 0 or if
  // they coincide, we just TODO.
  QVector3<T> *normals = new QVector3<T>[nvertices]; 
  for (int vi = 0; vi < nvertices; ++vi) {
    if (fores[vi].GetSquaredLength() < EPSILON && afts[vi].GetSquaredLength() < EPSILON) {
      throw MessagedException("no good vecs");
    } else if (fores[vi].GetSquaredLength() < EPSILON) {
      normals[vi] = afts[vi] * (T) -1;
    } else if (afts[vi].GetSquaredLength() < EPSILON) {
      normals[vi] = fores[vi];
    } else if (fabs(fores[vi].Dot(afts[vi])) > 0.999999f) {
      normals[vi] = afts[vi] * (T) -1;
    } else {
      QVector3<T> one_way = fores[vi] + afts[vi];
      QVector3<T> other_way = fores[vi].Cross(afts[vi]);
      normals[vi] = other_way.Cross(one_way);
      normals[vi].Normalize();
      if (vi == 0) {
        normals[vi] *= (T) -1;
      }
    }
  }

  // See if normals[0] has a 0 somewhere.
  int zero_dim = -1;
  for (int i = 0; i < 3 && zero_dim == -1; ++i) {
    if (fabs(normals[0][i]) < 1.0e-6f) {
      zero_dim = i;
    }
  }

  QVector3<T> away = normals[0].GetPerpendicular();

  // Create a new normal without the zeroed dimension.
  if (zero_dim >= 0) {
    QVector2<T> projected_normal;
    for (int d = 0, i = 0; d < 3; ++d) {
      if (d != zero_dim) {
        projected_normal[i] = normals[0][d];
        ++i;
      }
    }

    QVector2<T> projected_away = projected_normal.GetPerpendicular();
    for (int d = 0, i = 0; d < 3; ++d) {
      if (d != zero_dim) {
        away[d] = projected_away[i];
        ++i;
      } else {
        away[d] = 0.0f;
      }
    }
  }

  away = QMatrix4<float>::GetRotate(twist, normals[0]) * away;

  // Choose arbitrary away axis.
  /* QVector3<T> away = normals[0].GetPerpendicular(); */
  if (is_adaptive_radius) {
    away *= (*this)(0)[3];
  } else {
    away *= radius;
  }

  vector<QVector3<T> > cap_neighbors[2];
  vector<QVector3<T> > vertices;

  bool is_rounded = false;
  if (is_rounded) {
    for (int vi = 0; vi < nvertices; ++vi) {
      QVector3<T> middle = fores[vi] + afts[vi];
      middle.Normalize();

      if (is_adaptive_radius) {
        radius = (*this)(vi)[3];
      }

      QVector3<T> pivot = nodes[vi] + middle * radius;
      QVector3<T> fore_point = nodes[vi] + fores[vi] * fores[vi].Dot(pivot - nodes[vi]) / fores[vi].Dot(fores[vi]);
      QVector3<T> aft_point = nodes[vi] + afts[vi] * afts[vi].Dot(pivot - nodes[vi]) / afts[vi].Dot(afts[vi]);

      QVector3<T> pre = aft_point - pivot;
      QVector3<T> post = fore_point - pivot;
      pre.Normalize();
      post.Normalize();

      QVector3<T> rotation_axis = pre.Cross(post);
      rotation_axis.Normalize();

      T bend_degrees = acos(pre.Dot(post)) * 180 / td::PI; 
      int nrotates = 10;
      T delta = bend_degrees / (nrotates - 1);
      QMatrix4<T> rotator = QMatrix4<T>::GetTranslate(pivot[0], pivot[1], pivot[2]) *
                            QMatrix4<T>::GetRotate(delta, rotation_axis) *
                            QMatrix4<T>::GetTranslate(-pivot[0], -pivot[1], -pivot[2]);
      QVector3<float> slice_center = aft_point;
      QVector3<float> axis = afts[vi] * (T) -1;
      QVector3<float> away = fores[vi].Cross(afts[vi]);
      away.Normalize();
      away *= radius;

      for (int i = 0; i < nrotates; ++i) {
        std::cout << "(" << slice_center[0] << ", " << slice_center[1] << ")" << ((i < nrotates - 1) ? "," : "");
        for (int si = 0; si < nstops; ++si) {
          QVector3<T> away_neighbor = QMatrix4<float>::GetRotate(si * 360.0f / nstops, axis) * away + slice_center;
          vertices.push_back(away_neighbor);
        }
        slice_center = rotator * slice_center;
        axis = QVector3<T>(rotator * QVector4<T>(axis, 0));
      }
      std::cout << "" << std::endl;
    }
  } else {
    // Spin around current node and drop neighbors in around it.
    for (int si = 0; si < nstops; ++si) {
      QVector3<T> away_neighbor = QMatrix4<float>::GetRotate(si * 360.0f / nstops, normals[0]) * away + nodes[0];
      vertices.push_back(away_neighbor);
      if (is_open) {
        cap_neighbors[0].push_back(away_neighbor);
      }
    }

    // for each remaining node
    //   make plane (point as node, normal as fore + aft)
    //   axis = node - previous node
    //   for each stop
    //     intersect line (prev, axis) with plane

    for (int vi = 1; vi < nvertices; ++vi) {
      Plane<T, 3> plane(nodes[vi], normals[vi]);

      for (int si = 0; si < nstops; ++si) {
        QVector3<T> previous_away_neighbor(vertices[si + (vi - 1) * nstops]);
        Line<T, 3> line(previous_away_neighbor, afts[vi] * (T) -1);

        QVector3<T> intersection_point;
        plane.Intersect(line, intersection_point);
        vertices.push_back(intersection_point);

        if (vi == this->GetElementCount() - 1 && is_open) {
          cap_neighbors[1].push_back(intersection_point);
        }
      }
    }
  }

  delete[] nodes;
  delete[] normals;
  delete[] afts;
  delete[] fores;

  NField<float, 2> img(QVector2<int>(nstops, vertices.size() / nstops), 3);
  NFieldIterator<2> c(img.GetDimensions());
  int i = 0;
  while (c.HasNext()) {
    c.Next();
    memcpy(img(c), &vertices[i][0], sizeof(T) * 3);
    ++i;
  }

  Trimesh *mesh = Trimesh::GetParametric(img, true, !is_open);

  if (is_open) {
    for (int ci = 0; ci < 2; ++ci) {
      int vi = ci == 0 ? 0 : (this->GetElementCount() - 1);
      Trimesh *cap = new Trimesh(cap_neighbors[ci].size() + 1, nstops);

      float *position = cap->GetPositions();
      memcpy(position, (*this)(vi), sizeof(T) * 3);
      position += 3;
      for (unsigned int i = 0; i < cap_neighbors[ci].size(); ++i) {
        memcpy(position, &cap_neighbors[ci][i][0], sizeof(T) * 3);
        position += 3;
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
struct TriangulateBend {
  QVector3<T> vertex3;
  QVector2<T> vertex2;

  int array_index;
  int heap_index;
  int prev_index;
  int next_index;

  float theta; 

  TriangulateBend(const QVector3<T> vertex3,
                  const QVector2<T> vertex2,
                  int array_index) :
    vertex3(vertex3),
    vertex2(vertex2),
    array_index(array_index),
    heap_index(-1),
    prev_index(-1),
    next_index(-1) {
  }

  void UpdateTheta(const vector<TriangulateBend<T> >& bends) {
    QVector2<T> fro = bends[prev_index].vertex2 - vertex2;
    QVector2<T> to = bends[next_index].vertex2 - vertex2;

    fro.Normalize();
    to.Normalize();
    
    theta = acos(fro.Dot(to));

    float signed_area = to[0] * fro[1] - to[1] * fro[0];
    if (signed_area < 0.0f) {
      theta = 360.0f - theta;
    }
  }
};

/* ------------------------------------------------------------------------- */

template<typename T>
int Compare(const TriangulateBend<T>& a, const TriangulateBend<T>& b) {
  float diff = a.theta - b.theta;
  if (diff < 0) {
    return -1;
  } else if (diff > 0) {
    return 1;
  } else {
    return 0;
  }
}

/* ------------------------------------------------------------------------- */

template<class T>
Trimesh *Polyline<T>::Triangulate() const {
  // Assumes polyline traces planar polygon.

  // Solving this problem is a lot easier in two dimensions. We project the
  // triangle down to two dimensions. A simple way to do this is to get the
  // polygon's normal and drop the dimension of the component with the largest
  // magnitude.
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
  
  const int nvertices = this->GetElementCount();

  vector<TriangulateBend<T> > bends;
  MinHeap<TriangulateBend<T> > queue;
  vector<QVector3<float> > positions;
  vector<QVector3<int> > faces;

  // Assemble data structures.
  for (int i = 0; i < nvertices; ++i) {
    positions.push_back(QVector3<float>((*this)(i)));
    bends.push_back(TriangulateBend<T>(QVector3<float>((*this)(i)), QVector2<float>((*this)(i)[d[0]], (*this)(i)[d[1]]), i));
    bends[i].prev_index = (i + nvertices - 1) % nvertices;
    bends[i].next_index = (i + 1) % nvertices;
  }

  for (int i = 0; i < nvertices; ++i) {
    bends[i].UpdateTheta(bends);
    queue.Add(&bends[i]);
  }

  while (queue.Size() > 2) {
    TriangulateBend<T> *bend = queue.Remove();
    
    // Route neighbors around this vertex.
    bends[bend->prev_index].next_index = bend->next_index;
    bends[bend->next_index].prev_index = bend->prev_index;

    // emit bend->prev, bend.vertex, bend.next
    faces.push_back(QVector3<int>(bend->prev_index, bend->array_index, bend->next_index));

    bends[bend->prev_index].UpdateTheta(bends);
    queue.ReheapUp(bends[bend->prev_index].heap_index);
    queue.ReheapDown(bends[bend->prev_index].heap_index);

    bends[bend->next_index].UpdateTheta(bends);
    queue.ReheapUp(bends[bend->next_index].heap_index);
    queue.ReheapDown(bends[bend->next_index].heap_index);
  }

  return new Trimesh(positions, faces);;
}

/* ------------------------------------------------------------------------- */

template<class T>
Trimesh *Polyline<T>::Extrude(const QVector3<T> axis, T distance, const QMatrix4<float>& xform) const {
  Trimesh *cap_a = this->Triangulate(); 
  Trimesh *cap_b = new Trimesh(*cap_a);
  cap_b->ReverseWinding();
  *cap_b += axis * distance;
  *cap_b *= xform;

  const int nvertices_in_cap = cap_a->GetVertexCount();

  // Replicate geometry from two caps. We can just share the vertices already
  // in the caps, because we want the normals on the sides and caps to be
  // distinct. We don't need the faces. We'll get those from the separate
  // cap meshes.
  vector<QVector3<float> > positions; 
  vector<QVector3<int> > faces; 

  // Cap A vertices
  float *cap_positions = cap_a->GetPositions();
  float *cap_position = cap_positions;
  for (int i = 0; i < nvertices_in_cap; ++i, cap_position += 3) {
    positions.push_back(QVector3<float>(cap_position));
  }

  // Cap B vertices
  cap_positions = cap_b->GetPositions();
  cap_position = cap_positions;
  for (int i = 0; i < nvertices_in_cap; ++i, cap_position += 3) {
    positions.push_back(QVector3<float>(cap_position));
  }

  // connect faces across caps. 
  // *-----*
  //      /
  //    /
  //  /
  // *-----*
  //
  // i, (i + 1) % nvertices_in_cap, (i + 1) % nvertices_in_cap + nvertices_in_cap
  // i, (i + 1) % nvertices_in_cap + nvertices_in_cap + nvertices_in_cap, i + nvertices_in_cap

  for (int i = 0; i < nvertices_in_cap; ++i) {
    faces.push_back(QVector3<int>(i, (i + nvertices_in_cap - 1) % nvertices_in_cap, (i + nvertices_in_cap - 1) % nvertices_in_cap + nvertices_in_cap));
    faces.push_back(QVector3<int>(i, (i + nvertices_in_cap - 1) % nvertices_in_cap + nvertices_in_cap, i + nvertices_in_cap));
  }

  Trimesh *extruded = new Trimesh(positions, faces);
  *extruded += *cap_a;
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

}

#endif
