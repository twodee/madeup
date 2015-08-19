#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "twodee/QMath.h"
#include "twodee/QMatrix4.h"
#include "twodee/QVector4.h"

/* ------------------------------------------------------------------------- */

TEST(QMath, MatrixSquareMultiplication) {
  int buffer_a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  td::QMatrix4<int> a(buffer_a);

  int buffer_b[] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  td::QMatrix4<int> b(buffer_b);

  int buffer_c[] = {304, 358, 412, 466, 208, 246, 284, 322, 112, 134, 156, 178, 16, 22, 28, 34};
  td::QMatrix4<int> ans(buffer_c);

  td::QMatrix4<int> product = a * b;

  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      ASSERT_EQ(ans(r, c), product(r, c)) << "product(" << r << "," << c << ") return fails";
    }
  }
}

/* ------------------------------------------------------------------------- */

TEST(QMath, MatrixMultiplication) {
  int buffer_a[] = {2, 3};
  td::QMatrix<int, 2, 1> a(buffer_a);

  int buffer_b[] = {7, 1, 3};
  td::QMatrix<int, 1, 3> b(buffer_b);

  int buffer_c[] = {14, 21, 2, 3, 6, 9};
  td::QMatrix<int, 2, 3> ans(buffer_c);

  td::QMatrix<int, 2, 3> product = a * b;

  for (int r = 0; r < 2; ++r) {
    for (int c = 0; c < 3; ++c) {
      ASSERT_EQ(ans(r, c), product(r, c)) << "product(" << r << "," << c << ") return fails";
    }
  }
}

/* ------------------------------------------------------------------------- */

TEST(QMath, MatrixVectorMultiplication) {
  int buffer_a[] = {2, 4, 1, 5, 3, 6};
  td::QMatrix<int, 2, 3> a(buffer_a);
  td::QVector3<int> v(1, 2, 3);
  td::QVector<int, 2> ans = a * v;
  ASSERT_EQ(13, ans[0]) << "product[0] return fails";
  ASSERT_EQ(32, ans[1]) << "product[1] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QMath, MatrixPointMultiplication) {
  int buffer_a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  td::QSquareMatrix<int, 3> a(buffer_a);
  td::QVector<int, 2> v;
  v[0] = 8;
  v[1] = 9;

  td::QVector<int, 2> ans = a * v;

  ASSERT_EQ(51, ans[0]) << "product[0] return fails";
  ASSERT_EQ(69, ans[1]) << "product[1] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QMath, Ortho) {
  float l = -3, r = 2, b = -4, t = 5, n = -10, f = 20;

  td::QMatrix4<float> m = td::QMatrix4<float>::GetOrthographic(l, r, b, t, n, f);
  td::QVector3<float> lbn = m * td::QVector3<float>(l, b, -n); //, 1.0f);
  td::QVector3<float> rtf = m * td::QVector3<float>(r, t, -f); //, 1.0f);

  ASSERT_EQ(-1, lbn[0]) << "ortho fails";
  ASSERT_EQ(-1, lbn[1]) << "ortho fails";
  ASSERT_EQ(-1, lbn[2]) << "ortho fails";
  ASSERT_EQ(1, rtf[0]) << "ortho fails";
  ASSERT_EQ(1, rtf[1]) << "ortho fails";
  ASSERT_EQ(1, rtf[2]) << "ortho fails";

  td::QMatrix4<float> minv = td::QMatrix4<float>::GetOrthographicInverse(l, r, b, t, n, f);
  lbn = minv * td::QVector3<float>(-1.0f);
  rtf = minv * td::QVector3<float>(1.0f);

  ASSERT_EQ(l, lbn[0]) << "ortho inverse fails";
  ASSERT_EQ(b, lbn[1]) << "ortho inverse fails";
  ASSERT_EQ(f, lbn[2]) << "ortho inverse fails";
  ASSERT_EQ(r, rtf[0]) << "ortho inverse fails";
  ASSERT_EQ(t, rtf[1]) << "ortho inverse fails";
  ASSERT_EQ(n, rtf[2]) << "ortho inverse fails";
}

/* ------------------------------------------------------------------------- */

#if 0
TEST(QMath, Perspective6) {
  float l = -3, r = 2, b = -4, t = 5, n = 0.1f, f = 20;

  td::QMatrix4<float> m = td::QMatrix4<float>::GetPerspective(l, r, b, t, n, f);
  td::QVector4<float> lbn = m * td::QVector4<float>(l, b, n, 1.0f);
  lbn /= lbn[3];
  td::QVector4<float> rtf = m * td::QVector4<float>(r, t, f, 1.0f);
  rtf /= rtf[3];

  ASSERT_FLOAT_EQ(-1, lbn[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, lbn[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, lbn[2]) << "perspective fails";
  ASSERT_EQ(1, rtf[0]) << "perspective fails";
  ASSERT_EQ(1, rtf[1]) << "perspective fails";
  ASSERT_EQ(1, rtf[2]) << "perspective fails";
}
#endif

/* ------------------------------------------------------------------------- */

TEST(QMath, Perspective4) {
  float fov_y = 30.0f, aspect = 1.0f, n = 0.1f, f = 20;
  float theta = fov_y * 0.5f * td::PI / 180.0f;

  float tn = n * tan(theta);
  float bn = -tn;
  float rn = tn * aspect;
  float ln = -rn;

  float tf = f * tan(theta);
  float bf = -tf;
  float rf = tf * aspect;
  float lf = -rf;

  td::QMatrix4<float> m = td::QMatrix4<float>::GetPerspective(fov_y, aspect, n, f);

  // Near
  td::QVector4<float> lbn = m * td::QVector4<float>(ln, bn, -n, 1.0f);
  lbn /= lbn[3];
  ASSERT_FLOAT_EQ(-1, lbn[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, lbn[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, lbn[2]) << "perspective fails";

  td::QVector4<float> rbn = m * td::QVector4<float>(rn, bn, -n, 1.0f);
  rbn /= rbn[3];
  ASSERT_FLOAT_EQ(1, rbn[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, rbn[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, rbn[2]) << "perspective fails";

  td::QVector4<float> ltn = m * td::QVector4<float>(ln, tn, -n, 1.0f);
  ltn /= ltn[3];
  ASSERT_FLOAT_EQ(-1, ltn[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(1, ltn[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, ltn[2]) << "perspective fails";

  td::QVector4<float> rtn = m * td::QVector4<float>(rn, tn, -n, 1.0f);
  rtn /= rtn[3];
  ASSERT_FLOAT_EQ(1, rtn[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(1, rtn[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, rtn[2]) << "perspective fails";

  // Far
  td::QVector4<float> lbf = m * td::QVector4<float>(lf, bf, -f, 1.0f);
  lbf /= lbf[3];
  ASSERT_FLOAT_EQ(-1, lbf[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, lbf[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(1, lbf[2]) << "perspective fails";

  td::QVector4<float> rbf = m * td::QVector4<float>(rf, bf, -f, 1.0f);
  rbf /= rbf[3];
  ASSERT_FLOAT_EQ(1, rbf[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(-1, rbf[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(1, rbf[2]) << "perspective fails";

  td::QVector4<float> ltf = m * td::QVector4<float>(lf, tf, -f, 1.0f);
  ltf /= ltf[3];
  ASSERT_FLOAT_EQ(-1, ltf[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(1, ltf[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(1, ltf[2]) << "perspective fails";

  td::QVector4<float> rtf = m * td::QVector4<float>(rf, tf, -f, 1.0f);
  rtf /= rtf[3];
  ASSERT_FLOAT_EQ(1, rtf[0]) << "perspective fails";
  ASSERT_FLOAT_EQ(1, rtf[1]) << "perspective fails";
  ASSERT_FLOAT_EQ(1, rtf[2]) << "perspective fails";
}

/* ------------------------------------------------------------------------- */
