#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "twodee/MessagedException.h"
#include "twodee/Scanner.h"

/* ------------------------------------------------------------------------- */

TEST(Scanner, ReadInts) {
  td::Scanner in("1 2 3", td::Scanner::SOURCE_STRING);
  ASSERT_TRUE(in.HasNextInt()) << "HasNextInt fails";
  int i = in.NextInt();
  ASSERT_EQ(1, i) << "NextInt fails";
  ASSERT_TRUE(in.HasNextInt()) << "HasNextInt fails";
  i = in.NextInt();
  ASSERT_EQ(2, i) << "NextInt fails";
  ASSERT_TRUE(in.HasNextInt()) << "HasNextInt fails";
  i = in.NextInt();
  ASSERT_EQ(3, i) << "NextInt fails";
  ASSERT_FALSE(in.HasNextInt()) << "HasNextInt fails";
}

/* ------------------------------------------------------------------------- */

TEST(Scanner, BadReadInt) {
  td::Scanner in("a 2 3", td::Scanner::SOURCE_STRING);
  ASSERT_FALSE(in.HasNextInt());
  ASSERT_THROW(in.NextInt(), td::MessagedException);
}

/* ------------------------------------------------------------------------- */

TEST(Scanner, ReadIntEmpty) {
  td::Scanner in("", td::Scanner::SOURCE_STRING);
  ASSERT_FALSE(in.HasNextInt());
  ASSERT_THROW(in.NextInt(), td::MessagedException);
}

/* ------------------------------------------------------------------------- */

TEST(Scanner, ReadString) {
  td::Scanner in("a 2 c", td::Scanner::SOURCE_STRING);
  ASSERT_TRUE(in.HasNext()) << "HasNext fails";
  ASSERT_FALSE(in.HasNextInt()) << "HasNextInt fails";
  std::string s = in.Next();
  ASSERT_EQ("a", s) << "Next fails";
  ASSERT_TRUE(in.HasNextInt()) << "HasNextInt fails";
  ASSERT_TRUE(in.HasNext()) << "HasNext fails";
  s = in.Next();
  ASSERT_EQ("2", s) << "Next fails";
  ASSERT_TRUE(in.HasNext()) << "HasNext fails";
  ASSERT_FALSE(in.HasNextInt()) << "HasNextInt fails";
  s = in.Next();
  ASSERT_EQ("c", s) << "Next fails";
  ASSERT_FALSE(in.HasNext()) << "HasNext fails";
  ASSERT_FALSE(in.HasNextInt()) << "HasNextInt fails";
}

/* ------------------------------------------------------------------------- */

TEST(Scanner, RewindWithoutMark) {
  td::Scanner in("1 2 3", td::Scanner::SOURCE_STRING);
  ASSERT_EQ(1, in.NextInt()) << "NextInt fails";
  ASSERT_EQ(2, in.NextInt()) << "NextInt fails";
  ASSERT_EQ(3, in.NextInt()) << "NextInt fails";
  in.Rewind();
  ASSERT_EQ(1, in.NextInt()) << "NextInt fails";
  ASSERT_EQ(2, in.NextInt()) << "NextInt fails";
  ASSERT_EQ(3, in.NextInt()) << "NextInt fails";
}

/* ------------------------------------------------------------------------- */

TEST(Scanner, RewindWithMark) {
  td::Scanner in("1 2 3", td::Scanner::SOURCE_STRING);
  ASSERT_EQ(1, in.NextInt()) << "NextInt fails";
  in.Mark();
  ASSERT_EQ(2, in.NextInt()) << "NextInt fails";
  ASSERT_EQ(3, in.NextInt()) << "NextInt fails";
  in.Rewind();
  ASSERT_EQ(2, in.NextInt()) << "NextInt fails";
  ASSERT_EQ(3, in.NextInt()) << "NextInt fails";
  in.Rewind();
  ASSERT_EQ(2, in.NextInt()) << "NextInt fails";
  ASSERT_EQ(3, in.NextInt()) << "NextInt fails";
}

/* ------------------------------------------------------------------------- */

TEST(Scanner, ReadLine) {
  td::Scanner in("foo dragon\nsnidely whiplash", td::Scanner::SOURCE_STRING);
  ASSERT_TRUE(in.HasNextLine());
  ASSERT_EQ("foo dragon", in.NextLine()) << "NextLine fails";
  ASSERT_TRUE(in.HasNextLine());
  ASSERT_EQ("snidely whiplash", in.NextLine()) << "NextLine fails";
  ASSERT_FALSE(in.HasNextLine());
  in.Rewind();
  ASSERT_TRUE(in.HasNextLine());
  ASSERT_EQ("foo dragon", in.NextLine()) << "NextLine fails";
}

/* ------------------------------------------------------------------------- */

TEST(Scanner, ReadFloats) {
  td::Scanner in("1 2.0 3.0e-3 5.", td::Scanner::SOURCE_STRING);
  ASSERT_TRUE(in.HasNextFloat()) << "HasNextFloat fails";
  float i = in.NextFloat();
  ASSERT_EQ(1.0f, i) << "NextFloat fails";
  ASSERT_TRUE(in.HasNextFloat()) << "HasNextFloat fails";
  i = in.NextFloat();
  ASSERT_EQ(2.0f, i) << "NextFloat fails";
  ASSERT_TRUE(in.HasNextFloat()) << "HasNextFloat fails";
  i = in.NextFloat();
  ASSERT_EQ(3.0e-3f, i) << "NextFloat fails";
  ASSERT_TRUE(in.HasNextFloat()) << "HasNextFloat fails";
  i = in.NextFloat();
  ASSERT_EQ(5.0f, i) << "NextFloat fails";
  ASSERT_FALSE(in.HasNextFloat()) << "HasNextFloat fails";
}

/* ------------------------------------------------------------------------- */

TEST(Scanner, ReadDoubles) {
  td::Scanner in("1 2.0 8.0e-3 5.", td::Scanner::SOURCE_STRING);
  ASSERT_TRUE(in.HasNextDouble()) << "HasNextDouble fails";
  float i = in.NextDouble();
  ASSERT_EQ(1, i) << "NextDouble fails";
  ASSERT_TRUE(in.HasNextDouble()) << "HasNextDouble fails";
  i = in.NextDouble();
  ASSERT_EQ(2.0, i) << "NextDouble fails";
  ASSERT_TRUE(in.HasNextDouble()) << "HasNextDouble fails";
  i = in.NextDouble();
  ASSERT_NEAR(8.0e-3, i, 1.0e-6) << "NextDouble fails";
  ASSERT_TRUE(in.HasNextDouble()) << "HasNextDouble fails";
  i = in.NextDouble();
  ASSERT_EQ(5.0, i) << "NextDouble fails";
  ASSERT_FALSE(in.HasNextDouble()) << "HasNextDouble fails";
}

/* ------------------------------------------------------------------------- */
