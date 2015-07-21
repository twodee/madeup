#ifndef TRIANGLE_H
#define TRIANGLE_H

namespace td {

/* ------------------------------------------------------------------------- */

template<typename T> class Triangle2 {
  public:
    Triangle2(const QVector2<T> &a,
              const QVector2<T> &b,
              const QVector2<T> &c);

    bool Contains(const QVector2<T> &other, T epsilon = 1.0e-3) const;

  private:
    QVector2<T> a;
    QVector2<T> b;
    QVector2<T> c;
};

/* ------------------------------------------------------------------------- */

template<typename T>
Triangle2<T>::Triangle2(const QVector2<T> &a,
                        const QVector2<T> &b,
                        const QVector2<T> &c) :
  a(a),
  b(b),
  c(c) {
}

/* ------------------------------------------------------------------------- */

template<typename T>
bool Triangle2<T>::Contains(const QVector2<T> &other,
                            T epsilon) const {
  QVector2<T> v0 = c - a;
  QVector2<T> v1 = b - a;
  QVector2<T> v2 = other - a;

  // Compute dot products
  float dot00 = v0.Dot(v0);
  float dot01 = v0.Dot(v1);
  float dot02 = v0.Dot(v2);
  float dot11 = v1.Dot(v1);
  float dot12 = v1.Dot(v2);
 
  // Compute barycentric coordinates
  float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
  float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
  
  // Check if point is in triangle
  return u >= -epsilon && v >= -epsilon && u + v < 1 + epsilon;
}

/* ------------------------------------------------------------------------- */

}

#endif
