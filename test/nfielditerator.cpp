#include <gtest/gtest.h>

#include "twodee/NFieldIterator.h"
#include "twodee/QVector2.h"
#include "twodee/QVector3.h"

/* ------------------------------------------------------------------------- */

TEST(NFieldIteratorTest, Defaults) {
  td::QVector2<int> max(3, 2);
  td::NFieldIterator<2> c(max);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(0, 0), c);
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(1, 0), c);
  ASSERT_TRUE(c.HasNext());
  std::cout << "c: " << c << std::endl;
  c.Next();
  std::cout << "c: " << c << std::endl;
  ASSERT_EQ(td::QVector2<int>(2, 0), c);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  std::cout << "c: " << c << std::endl;
  ASSERT_EQ(td::QVector2<int>(0, 1), c);
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(1, 1), c);
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(2, 1), c);

  ASSERT_FALSE(c.HasNext());
}

/* ------------------------------------------------------------------------- */

TEST(NFieldIteratorTest, NonDefaultMinMax) {
  td::QVector2<int> min(1, 2);
  td::QVector2<int> max(3, 4);
  td::NFieldIterator<2> c(max, min);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(min, c);
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(2, 2), c);
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(1, 3), c);
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(2, 3), c);
  ASSERT_FALSE(c.HasNext());
}

/* ------------------------------------------------------------------------- */

TEST(NFieldIteratorTest, Jump) {
  td::QVector2<int> max(4, 2);
  td::NFieldIterator<2> c(max, td::QVector2<int>(0, 0), td::QVector2<int>(2, 1));

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(0, 0), c);
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(2, 0), c);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(0, 1), c);
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(2, 1), c);

  ASSERT_FALSE(c.HasNext());
}

/* ------------------------------------------------------------------------- */

TEST(NFieldIteratorTest, ReverseSimple) {
  td::NFieldIterator<2> c(td::QVector2<int>(-1, -1),
                          td::QVector2<int>(0, 0),
                          td::QVector2<int>(-1, -1));
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(0, 0), c);
  ASSERT_FALSE(c.HasNext());
}

/* ------------------------------------------------------------------------- */

TEST(NFieldIteratorTest, ReverseLine) {
  td::NFieldIterator<2> c(td::QVector2<int>(-1, 1),
                          td::QVector2<int>(10, 0),
                          td::QVector2<int>(-2, 1));
  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(10, 0), c);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(8, 0), c);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(6, 0), c);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(4, 0), c);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(2, 0), c);

  ASSERT_TRUE(c.HasNext());
  c.Next();
  ASSERT_EQ(td::QVector2<int>(0, 0), c);

  ASSERT_FALSE(c.HasNext());
}

/* ------------------------------------------------------------------------- */

TEST(NFieldIteratorTest, ReverseGrid) {
  td::NFieldIterator<2> c(td::QVector2<int>(-1, -1),
                          td::QVector2<int>(100, 50),
                          td::QVector2<int>(-1, -1));
  for (int j = 50; j >= 0; --j) {
    for (int i = 100; i >= 0; --i) {
      ASSERT_TRUE(c.HasNext());
      c.Next();
      ASSERT_EQ(td::QVector2<int>(i, j), c);
    }
  }
  ASSERT_FALSE(c.HasNext());
}

/* ------------------------------------------------------------------------- */

