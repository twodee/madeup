#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "twodee/QMatrix4.h"

/* ------------------------------------------------------------------------- */

TEST(QMatrix4Test, CtorScalar) {
  td::QMatrix4<int> m(5);
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      if (r == c) {
        ASSERT_EQ(5, m(r, c)) << "m(" << r << "," << c << ") return fails";
      } else {
        ASSERT_EQ(0, m(r, c)) << "m(" << r << "," << c << ") return fails";
      }
    }
  }
}

/* ------------------------------------------------------------------------- */

TEST(QMatrix4Test, CtorArray) {
  int buffer[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  td::QMatrix4<int> m(buffer);
  for (int c = 0, i = 0; c < 4; ++c) {
    for (int r = 0; r < 4; ++r, ++i) {
      ASSERT_EQ(buffer[i], m(r, c)) << "m(" << r << "," << c << ") return fails";
    }
  }
}

/* ------------------------------------------------------------------------- */

TEST(QMatrix4Test, GetTranslate) {
  td::QMatrix4<int> m = td::QMatrix4<int>::GetTranslate(1, 2, 3);
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      if (r == c) {
        ASSERT_EQ(1, m(r, c)) << "Diagonal elements must be 1";
      } else if (c == 3) {
        ASSERT_EQ(r + 1, m(r, c)) << "Offset wrong";
      } else {
        ASSERT_EQ(0, m(r, c)) << "Non-diagonals, non-offsets must be 0";
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
