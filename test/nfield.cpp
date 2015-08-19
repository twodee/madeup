#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "twodee/NField.h"
#include "twodee/QVector2.h"

using namespace td;

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, DefaultCtor) {
  td::NField<int, 2> field;
  td::QVector<int, 2> dims;
  dims[0] = dims[1] = 0;
  ASSERT_EQ(dims, field.GetDimensions());
  ASSERT_EQ(NULL, field.GetData());
  ASSERT_EQ(0, field.GetChannelCount());
  ASSERT_EQ(0, field.GetElementCount());
  ASSERT_EQ(0, field.GetValueCount());
}

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, NondefaultCtor) {
  td::QVector<int, 2> dims;
  dims[0] = 5;
  dims[1] = 2;
  td::NField<int, 2> field(dims, 3);

  ASSERT_EQ(dims, field.GetDimensions());
  ASSERT_EQ(3, field.GetChannelCount());
  ASSERT_EQ(10, field.GetElementCount());
  ASSERT_EQ(30, field.GetValueCount());
}

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, Indexing) {
  td::NField<int, 2> field(td::QVector2<int>(2, 2), 2);

  field(0)[0] = 0;
  field(0)[1] = 1;
  field(1)[0] = 2;
  field(1)[1] = 3;
  field(2)[0] = 4;
  field(2)[1] = 5;
  field(3)[0] = 6;
  field(3)[1] = 7;

  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(i * 2 + 0, field(i)[0]);
    ASSERT_EQ(i * 2 + 1, field(i)[1]);
  }
}

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, MinMax) {
  td::NField<int, 2> field(td::QVector2<int>(2, 2), 2);

  field(0)[0] = 0;
  field(0)[1] = 1;
  field(1)[0] = 2;
  field(1)[1] = 3;
  field(2)[0] = 4;
  field(2)[1] = 5;
  field(3)[0] = 6;
  field(3)[1] = 7;

  field.ComputeMinMax();
  ASSERT_EQ(0, field.GetMinimum(0));
  ASSERT_EQ(6, field.GetMaximum(0));
  ASSERT_EQ(1, field.GetMinimum(1));
  ASSERT_EQ(7, field.GetMaximum(1));

  field.ComputeMinMax(false);
  ASSERT_EQ(0, field.GetMinimum(0));
  ASSERT_EQ(7, field.GetMaximum(0));
  ASSERT_EQ(0, field.GetMinimum(1));
  ASSERT_EQ(7, field.GetMaximum(1));
}

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, NormalizeFloat) {
  td::NField<float, 2> field(td::QVector2<int>(5, 1), 2);

  field(0)[0] = 10;
  field(0)[1] = 20;
  field(1)[0] = 30;
  field(1)[1] = 40;
  field(2)[0] = 50;
  field(2)[1] = 60;
  field(3)[0] = 70;
  field(3)[1] = 80;
  field(4)[0] = 90;
  field(4)[1] = 110;

  field.ComputeMinMax(false);
  field.Normalize();
  const float *data = field.GetData();
  ASSERT_EQ(0.0f, data[0]);
  ASSERT_EQ(0.1f, data[1]);
  ASSERT_EQ(0.2f, data[2]);
  ASSERT_EQ(0.3f, data[3]);
  ASSERT_EQ(0.4f, data[4]);
  ASSERT_EQ(0.5f, data[5]);
  ASSERT_EQ(0.6f, data[6]);
  ASSERT_EQ(0.7f, data[7]);
  ASSERT_EQ(0.8f, data[8]);
  ASSERT_EQ(1.0f, data[9]);
}

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, NormalizeUnsignedChar) {
  td::NField<unsigned char, 2> field(td::QVector2<int>(2, 2), 1);

  field(0)[0] = 10;
  field(1)[0] = 30;
  field(2)[0] = 60;
  field(3)[0] = 70;

  field.Normalize();

  const unsigned char *data = field.GetData();
  ASSERT_EQ((unsigned char) ((10 - 10) / 60.0f * 255.0f), data[0]);
  ASSERT_EQ((unsigned char) ((30 - 10) / 60.0f * 255.0f), data[1]);
  ASSERT_EQ((unsigned char) ((60 - 10) / 60.0f * 255.0f), data[2]);
  ASSERT_EQ((unsigned char) ((70 - 10) / 60.0f * 255.0f), data[3]);
}

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, GetSubfield) {
  td::NField<int, 1> field(td::QVector<int, 1>(5));
  *field(0) = 0;
  *field(1) = 1;
  *field(2) = 2;
  *field(3) = 3;
  *field(4) = 4;

  td::NField<int, 1> *subfield = field.GetSubfield(td::QVector<int, 1>(2), td::QVector<int, 1>(4));
  ASSERT_EQ((td::QVector<int, 1>(3)), subfield->GetDimensions());

  ASSERT_EQ(2, *(*subfield)(0));
  ASSERT_EQ(3, *(*subfield)(1));
  ASSERT_EQ(4, *(*subfield)(2));

  delete subfield;
}

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, GetSubfieldCopy) {
  td::NField<int, 2> field(td::QVector2<int>(5, 10));

  td::NField<int, 2> *subfield = field.GetSubfield(td::QVector2<int>(0, 0), td::QVector2<int>(4, 9));
  ASSERT_EQ((td::QVector2<int>(5, 10)), subfield->GetDimensions());

  for (int r = 0; r < 10; ++r) {
    for (int c = 0; c < 5; ++c) {
      td::QVector2<int> i(c, r);
      ASSERT_EQ(field(i)[0], (*subfield)(i)[0]);
    }
  }

  delete subfield;
}

/* ------------------------------------------------------------------------- */

TEST(NFieldTest, Transpose) {
  td::NField<int, 2> field(td::QVector2<int>(2, 3));
  field(QVector2<int>(0, 0))[0] = 1;
  field(QVector2<int>(1, 0))[0] = 2;
  field(QVector2<int>(0, 1))[0] = 10;
  field(QVector2<int>(1, 1))[0] = 20;
  field(QVector2<int>(0, 2))[0] = 100;
  field(QVector2<int>(1, 2))[0] = 200;

  // 100 200
  //  10  20
  //   1   2

  // 2 20 200
  // 1 10 100

  field.Transpose();
  ASSERT_EQ(3, field.GetDimensions()[0]);
  ASSERT_EQ(2, field.GetDimensions()[1]);

  ASSERT_EQ(1, field(QVector2<int>(0, 0))[0]);
  ASSERT_EQ(10, field(QVector2<int>(1, 0))[0]);
  ASSERT_EQ(100, field(QVector2<int>(2, 0))[0]);
  ASSERT_EQ(2, field(QVector2<int>(0, 1))[0]);
  ASSERT_EQ(20, field(QVector2<int>(1, 1))[0]);
  ASSERT_EQ(200, field(QVector2<int>(2, 1))[0]);
}

/* ------------------------------------------------------------------------- */

