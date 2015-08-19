#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "twodee/QSquareMatrix.h"

/* ------------------------------------------------------------------------- */

TEST(QSquareMatrixTest, CtorScalar) {
  td::QSquareMatrix<int, 3> v(5);
  for (int r = 0; r < 3; ++r) {
    for (int c = 0; c < 3; ++c) {
      if (r == c) {
        ASSERT_EQ(5, v(r, c)) << "v(" << r << "," << c << ") return fails";
      } else {
        ASSERT_EQ(0, v(r, c)) << "v(" << r << "," << c << ") return fails";
      }
    }
  }
}

/* ------------------------------------------------------------------------- */

TEST(QSquareMatrixTest, CtorArray) {
  int buffer[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  td::QSquareMatrix<int, 3> v(buffer);
  for (int c = 0, i = 0; c < 3; ++c) {
    for (int r = 0; r < 3; ++r, ++i) {
      ASSERT_EQ(buffer[i], v(r, c)) << "v(" << r << "," << c << ") return fails";
    }
  }
}

/* ------------------------------------------------------------------------- */
