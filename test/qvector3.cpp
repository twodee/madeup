#include <gtest/gtest.h>

#include "twodee/QVector3.h"

/* ------------------------------------------------------------------------- */

TEST(QVectorTest3, CtorScalar) {
  td::QVector3<int> v(5);
  ASSERT_EQ(5, v[0]) << "v(5)[0] return fails";
  ASSERT_EQ(5, v[1]) << "v(5)[1] return fails";
  ASSERT_EQ(5, v[2]) << "v(5)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest3, CtorFirstLast) {
  td::QVector3<int> v(5, 1);
  ASSERT_EQ(5, v[0]) << "v(5, 1)[0] return fails";
  ASSERT_EQ(5, v[1]) << "v(5, 1)[1] return fails";
  ASSERT_EQ(1, v[2]) << "v(5, 1)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest3, CtorPointer) {
  int src[] = {1, 2, 3, 4, 5};
  td::QVector3<int> v(src);
  ASSERT_EQ(1, v[0]) << "v({1,2,3,4,5})[0] return fails";
  ASSERT_EQ(2, v[1]) << "v({1,2,3,4,5})[1] return fails";
  ASSERT_EQ(3, v[2]) << "v({1,2,3,4,5})[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest3, CtorCopy) {
  td::QVector3<int> v1;
  v1[0] = 1;
  v1[1] = 6;
  v1[2] = 7;
  td::QVector3<int> v2(v1);
  ASSERT_EQ(v1[0], v2[0]) << "v2(v1)[0] return fails";
  ASSERT_EQ(v1[1], v2[1]) << "v2(v1)[1] return fails";
  ASSERT_EQ(v1[2], v2[2]) << "v2(v1)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest3, CtorFromLower) {
  td::QVector<int, 2> v2(2);
  td::QVector3<int> v3(v2, 3);
  ASSERT_EQ(2, v3[0]) << "v3(v2, 3)[0] return fails";
  ASSERT_EQ(2, v3[1]) << "v3(v2, 3)[1] return fails";
  ASSERT_EQ(3, v3[2]) << "v3(v2, 3)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest3, CtorFromHigher) {
  td::QVector<int, 4> v4;
  v4[0] = 1;
  v4[1] = 2;
  v4[2] = 3;
  v4[3] = 4;
  td::QVector3<int> v3(v4);
  ASSERT_EQ(1, v3[0]) << "v3(v4)[0] return fails";
  ASSERT_EQ(2, v3[1]) << "v3(v4)[1] return fails";
  ASSERT_EQ(3, v3[2]) << "v3(v4)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest3, CtorFromList) {
  td::QVector3<int> v(1, 2, 3);
  ASSERT_EQ(1, v[0]) << "v(1, 2, 3)[0] return fails";
  ASSERT_EQ(2, v[1]) << "v(1, 2, 3)[1] return fails";
  ASSERT_EQ(3, v[2]) << "v(1, 2, 3)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest3, Cross) {
  td::QVector3<int> v1;
  v1[0] = 1;
  v1[1] = 2;
  v1[2] = 3;
  td::QVector3<int> v2;
  v2[0] = -1;
  v2[1] = 2;
  v2[2] = 6;
  td::QVector3<int> v3 = v1.Cross(v2);
  ASSERT_EQ(6, v3[0]) << "cross-product fails";
  ASSERT_EQ(-9, v3[1]) << "cross-product fails";
  ASSERT_EQ(4, v3[2]) << "cross-product fails";
  v3 = v2.Cross(v1);
  ASSERT_EQ(-6, v3[0]) << "cross-product fails";
  ASSERT_EQ(9, v3[1]) << "cross-product fails";
  ASSERT_EQ(-4, v3[2]) << "cross-product fails";
}

/* ------------------------------------------------------------------------- */

