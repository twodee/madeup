#include <gtest/gtest.h>

#include "twodee/Camera.h"

/* ------------------------------------------------------------------------- */

TEST(CameraTest, CtorDefault) {
  td::Camera camera;
  ASSERT_EQ(td::QVector3<float>(0.0f), camera.GetFrom()) << "getFrom fails";
}

/* ------------------------------------------------------------------------- */

TEST(CameraTest, CtorLookAt) {
  td::QVector3<float> from(1.0f, 2.0f, 3.0f);
  td::QVector3<float> to(4.0f, 5.0f, 6.0f);
  td::QVector3<float> up(-1.0f, -2.0f, -3.0f);
  td::Camera camera(from, to, up);
  ASSERT_EQ(from, camera.GetFrom()) << "getFrom fails";
}

/* ------------------------------------------------------------------------- */

TEST(CameraTest, GetSetNearFar) {
  td::Camera camera;
  camera.SetPerspectiveNear(5.0f);
  ASSERT_EQ(5.0f, camera.GetPerspectiveNear());
  camera.SetPerspectiveFar(10.0f);
  ASSERT_EQ(10.0f, camera.GetPerspectiveFar());
}

/* ------------------------------------------------------------------------- */

TEST(CameraTest, Move) {
  td::Camera camera;
  camera.Move(1.0f, 2.0f, 3.0f);
  camera.Move(1.0f, 2.0f, 3.0f);
  td::QVector3<float> from(2.0f, 4.0f, 6.0f);
  ASSERT_EQ(from, camera.GetFrom());
}

/* ------------------------------------------------------------------------- */

TEST(CameraTest, Advance) {
  td::QVector3<float> from(0.0f);
  td::QVector3<float> up(0.0f, 1.0f, 0.0f);
  td::QVector3<float> to(1.0f, 0.0f, 0.0f);
  td::Camera camera(from, to, up);
  camera.Advance(5.0f);
  ASSERT_EQ(5.0f, camera.GetFrom()[0]);
  camera.Advance(-15.0f);
  ASSERT_EQ(-10.0f, camera.GetFrom()[0]);
}

/* ------------------------------------------------------------------------- */

TEST(CameraTest, Strafe) {
  td::QVector3<float> from(0.0f);
  td::QVector3<float> up(0.0f, 1.0f, 0.0f);
  td::QVector3<float> to(1.0f, 0.0f, 0.0f);
  td::Camera camera(from, to, up);
  camera.Strafe(5.0f);
  ASSERT_EQ(5.0f, camera.GetFrom()[2]);
  camera.Strafe(-15.0f);
  ASSERT_EQ(-10.0f, camera.GetFrom()[2]);
}

/* ------------------------------------------------------------------------- */

