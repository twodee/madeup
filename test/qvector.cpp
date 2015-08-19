#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "twodee/QVector.h"

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, CtorScalar) {
  td::QVector<int, 2> v(5);
  ASSERT_EQ(5, v[0]) << "v(5)[0] return fails";
  ASSERT_EQ(5, v[1]) << "v(5)[1] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, CtorFirstLast) {
  td::QVector<int, 3> v(5, 1);
  ASSERT_EQ(5, v[0]) << "v(5, 1)[0] return fails";
  ASSERT_EQ(5, v[1]) << "v(5, 1)[1] return fails";
  ASSERT_EQ(1, v[2]) << "v(5, 1)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, CtorPointer) {
  int src[] = {1, 2, 3, 4, 5};
  td::QVector<int, 4> v(src);
  ASSERT_EQ(1, v[0]) << "v({1,2,3,4,5})[0] return fails";
  ASSERT_EQ(2, v[1]) << "v({1,2,3,4,5})[1] return fails";
  ASSERT_EQ(3, v[2]) << "v({1,2,3,4,5})[2] return fails";
  ASSERT_EQ(4, v[3]) << "v({1,2,3,4,5})[3] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, CtorCopy) {
  td::QVector<int, 3> v1;
  v1[0] = 1;
  v1[1] = 6;
  v1[2] = 7;
  td::QVector<int, 3> v2(v1);
  ASSERT_EQ(v1[0], v2[0]) << "v2(v1)[0] return fails";
  ASSERT_EQ(v1[1], v2[1]) << "v2(v1)[1] return fails";
  ASSERT_EQ(v1[2], v2[2]) << "v2(v1)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, CtorFromLower) {
  td::QVector<int, 2> v2(2);
  td::QVector<int, 3> v3(v2, 3);
  ASSERT_EQ(2, v3[0]) << "v3(v2, 3)[0] return fails";
  ASSERT_EQ(2, v3[1]) << "v3(v2, 3)[1] return fails";
  ASSERT_EQ(3, v3[2]) << "v3(v2, 3)[2] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, CtorFromHigher) {
  td::QVector<int, 3> v3(3);
  td::QVector<int, 2> v2(v3);
  ASSERT_EQ(3, v3[0]) << "v2(v3)[0] return fails";
  ASSERT_EQ(3, v3[1]) << "v2(v3)[1] return fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, SquaredLength) {
  td::QVector<int, 3> v3(0);
  ASSERT_EQ(0, v3.GetSquaredLength()) << "Wrong squared length";
  td::QVector<int, 1> v1(5);
  ASSERT_EQ(25, v1.GetSquaredLength()) << "Wrong squared length";
  td::QVector<int, 2> v2(2);
  ASSERT_EQ(8, v2.GetSquaredLength()) << "Wrong squared length";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, Length) {
  td::QVector<int, 3> v3(0);
  ASSERT_EQ(0, v3.GetLength()) << "Wrong length";
  v3[0] = v3[1] = v3[2] = 1;
  ASSERT_EQ(1, v3.GetLength()) << "Wrong length";
  td::QVector<float, 2> v2;
  v2[0] = 3.0f;
  v2[1] = 4.0f;
  ASSERT_EQ(5.0f, v2.GetLength()) << "Wrong length";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, Dot) {
  td::QVector<int, 3> v1;
  v1[0] = 1;
  v1[1] = 2;
  v1[2] = -3;
  td::QVector<int, 3> v2;
  v2[0] = 2;
  v2[1] = 0;
  v2[2] = 3;
  ASSERT_EQ(-7, v1.Dot(v2)) << "Wrong dot product";
  ASSERT_EQ(14, v1.Dot(v1)) << "Wrong dot product";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, VectorVectorOps) {
  td::QVector<int, 2> v1;
  v1[0] = 10;
  v1[1] = 8;
  td::QVector<int, 2> v2;
  v2[0] = 2;
  v2[1] = 1;

  // Sum
  td::QVector<int, 2> v3 = v1 + v2;
  ASSERT_EQ(12, v3[0]) << "v1 + v2 fails";
  ASSERT_EQ(9, v3[1]) << "v1 + v2 fails";

  // Difference
  v3 = v1 - v2;
  ASSERT_EQ(8, v3[0]) << "v1 - v2 fails";
  ASSERT_EQ(7, v3[1]) << "v1 - v2 fails";

  // Product
  v3 = v1 * v2;
  ASSERT_EQ(20, v3[0]) << "v1 * v2 fails";
  ASSERT_EQ(8, v3[1]) << "v1 * v2 fails";

  // Quotient
  v3 = v1 / v2;
  ASSERT_EQ(5, v3[0]) << "v1 / v2 fails";
  ASSERT_EQ(8, v3[1]) << "v1 / v2 fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, VectorScalarOps) {
  td::QVector<int, 2> v1;
  v1[0] = 10;
  v1[1] = 8;
  int scalar = 2;

  // Sum
  td::QVector<int, 2> v3 = v1 + scalar;
  ASSERT_EQ(12, v3[0]) << "v1 + scalar fails";
  ASSERT_EQ(10, v3[1]) << "v1 + scalar fails";

  // Difference
  v3 = v1 - scalar;
  ASSERT_EQ(8, v3[0]) << "v1 - scalar fails";
  ASSERT_EQ(6, v3[1]) << "v1 - scalar fails";

  // Product
  v3 = v1 * scalar;
  ASSERT_EQ(20, v3[0]) << "v1 * scalar fails";
  ASSERT_EQ(16, v3[1]) << "v1 * scalar fails";

  // Quotient
  v3 = v1 / scalar;
  ASSERT_EQ(5, v3[0]) << "v1 / scalar fails";
  ASSERT_EQ(4, v3[1]) << "v1 / scalar fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, ScalarVectorOps) {
  int scalar = 4;
  td::QVector<int, 2> v1;
  v1[0] = 2;
  v1[1] = 1;

  // Sum
  td::QVector<int, 2> v3 = scalar + v1;
  ASSERT_EQ(6, v3[0]) << "scalar + v1 fails";
  ASSERT_EQ(5, v3[1]) << "scalar + v1 fails";

  // Difference
  v3 = scalar - v1;
  ASSERT_EQ(2, v3[0]) << "scalar - v1 fails";
  ASSERT_EQ(3, v3[1]) << "scalar - v1 fails";

  // Product
  v3 = scalar * v1;
  ASSERT_EQ(8, v3[0]) << "scalar * v1 fails";
  ASSERT_EQ(4, v3[1]) << "scalar * v1 fails";

  // Quotient
  v3 = scalar / v1;
  ASSERT_EQ(2, v3[0]) << "scalar / v1 fails";
  ASSERT_EQ(4, v3[1]) << "scalar / v1 fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, SelfVectorOps) {
  td::QVector<int, 2> v1;
  v1[0] = 10;
  v1[1] = 8;
  td::QVector<int, 2> v2;
  v2[0] = 2;
  v2[1] = 1;

  // Sum
  td::QVector<int, 2> v3 = v1;
  v3 += v2;
  ASSERT_EQ(12, v3[0]) << "+= fails";
  ASSERT_EQ(9, v3[1]) << "+= fails";

  // Difference
  v3 = v1;
  v3 -= v2;
  ASSERT_EQ(8, v3[0]) << "-= fails";
  ASSERT_EQ(7, v3[1]) << "-= fails";

  // Product
  v3 = v1;
  v3 *= v2;
  ASSERT_EQ(20, v3[0]) << "*= fails";
  ASSERT_EQ(8, v3[1]) << "*= fails";

  // Quotient
  v3 = v1;
  v3 /= v2;
  ASSERT_EQ(5, v3[0]) << "/= fails";
  ASSERT_EQ(8, v3[1]) << "/= fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, SelfScalarOps) {
  td::QVector<int, 2> v1;
  v1[0] = 10;
  v1[1] = 8;
  int scalar = 2;

  // Sum
  td::QVector<int, 2> v3 = v1;
  v3 += scalar;
  ASSERT_EQ(12, v3[0]) << "v1 + scalar fails";
  ASSERT_EQ(10, v3[1]) << "v1 + scalar fails";

  // Difference
  v3 = v1;
  v3 -= scalar;
  ASSERT_EQ(8, v3[0]) << "v1 - scalar fails";
  ASSERT_EQ(6, v3[1]) << "v1 - scalar fails";

  // Product
  v3 = v1;
  v3 *= scalar;
  ASSERT_EQ(20, v3[0]) << "v1 * scalar fails";
  ASSERT_EQ(16, v3[1]) << "v1 * scalar fails";

  // Quotient
  v3 = v1;
  v3 /= scalar;
  ASSERT_EQ(5, v3[0]) << "v1 / scalar fails";
  ASSERT_EQ(4, v3[1]) << "v1 / scalar fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, StreamOut) {
  td::QVector<int, 3> v;
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;
  std::stringstream ss(std::stringstream::out);
  ss << v;
  std::string actual = ss.str();
  ASSERT_EQ("1 2 3", actual);
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, StreamOutChar) {
  td::QVector<char, 3> v;
  v[0] = 'a';
  v[1] = '\1';
  v[2] = '@';
  std::stringstream ss(std::stringstream::out);
  ss << v;
  std::string actual = ss.str();
  ASSERT_EQ("a \1 @", actual);
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, StreamIn) {
  std::stringstream ss("1 2 3", std::stringstream::in);
  td::QVector<int, 3> actual;
  ss >> actual;
  td::QVector<int, 3> expected;
  expected[0] = 1;
  expected[1] = 2;
  expected[2] = 3;
  ASSERT_EQ(expected, actual) << "Stream in (>>) fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, StreamInChar) {
  std::stringstream ss("\1 @ 0", std::stringstream::in);
  td::QVector<char, 3> actual;
  ss >> actual;
  td::QVector<char, 3> expected;
  expected[0] = '\1';
  expected[1] = '@';
  expected[2] = '0';
  ASSERT_EQ(expected, actual) << "Stream in (>>) fails";
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, IndexOutOfBounds) {
  td::QVector<int, 2> v(5);
  ASSERT_DEATH(v[-1], "Assertion.*failed");
  ASSERT_DEATH(v[2], "Assertion.*failed");
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, SelfProduct) {
  td::QVector<int, 2> v(5);
  ASSERT_EQ(25, v.SelfProduct());

  td::QVector<int, 3> v3;
  v3[0] = 2;
  v3[1] = 3;
  v3[2] = 4;
  ASSERT_EQ(24, v3.SelfProduct());
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, Abs) {
  td::QVector<int, 3> n;
  n[0] = -1;
  n[1] = -2;
  n[2] = -3;
  td::QVector<int, 3> p = n.Abs();
  ASSERT_EQ(1, p[0]);
  ASSERT_EQ(2, p[1]);
  ASSERT_EQ(3, p[2]);

  p = p.Abs();
  ASSERT_EQ(1, p[0]);
  ASSERT_EQ(2, p[1]);
  ASSERT_EQ(3, p[2]);
}

/* ------------------------------------------------------------------------- */

TEST(QVectorTest, GetReverse) {
  td::QVector<int, 3> n;
  n[0] = -1;
  n[1] = -2;
  n[2] = -3;
  td::QVector<int, 3> r = n.GetReverse();
  ASSERT_EQ(-3, r[0]);
  ASSERT_EQ(-2, r[1]);
  ASSERT_EQ(-1, r[2]);
}

/* ------------------------------------------------------------------------- */

