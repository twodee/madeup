#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "twodee/QMatrix.h"

/* ------------------------------------------------------------------------- */

TEST(QMatrixTest, CtorScalar) {
  td::QMatrix<int, 2, 3> v(5);
  for (int r = 0; r < 2; ++r) {
    for (int c = 0; c < 3; ++c) {
      ASSERT_EQ(5, v(r, c)) << "v(" << r << "," << c << ") return fails";
    }
  }
}

/* ------------------------------------------------------------------------- */

TEST(QMatrixTest, CtorArray) {
  int buffer[] = {0, 1, 2, 3, 4, 5};
  td::QMatrix<int, 3, 2> v(buffer);
  for (int c = 0, i = 0; c < 2; ++c) {
    for (int r = 0; r < 3; ++r, ++i) {
      ASSERT_EQ(buffer[i], v(r, c)) << "v(" << r << "," << c << ") return fails";
    }
  }
}

/* ------------------------------------------------------------------------- */

TEST(QMatrixTest, IndexOutOfBounds) {
  td::QMatrix<int, 2, 3> v(5);
  ASSERT_DEATH(v(-1, 0), "Assertion.*failed");
  ASSERT_DEATH(v(1, -1), "Assertion.*failed");
  ASSERT_DEATH(v(0, 3), "Assertion.*failed");
  ASSERT_DEATH(v(2, 1), "Assertion.*failed");
}

/* ------------------------------------------------------------------------- */

TEST(QMatrixTest, StreamIn) {
  std::stringstream ss("1 2 3", std::stringstream::in);
  td::QMatrix<int, 1, 3> actual;
  ss >> actual;
  td::QMatrix<int, 1, 3> expected;
  expected(0, 0) = 1;
  expected(0, 1) = 2;
  expected(0, 2) = 3;
  ASSERT_EQ(expected, actual) << "Stream in (>>) fails";
}

/* ------------------------------------------------------------------------- */
