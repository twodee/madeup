#include "MadeupCanvas.h"
#include <iostream>

using std::string;

/* ------------------------------------------------------------------------- */

MadeupCanvas::MadeupCanvas(QWidget *parent) :
  QOpenGLWidget(parent),
  auto_rotate_timer(this) {
  QSurfaceFormat format;
  format.setVersion(3, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);

  // Multisampling seems to mess up glReadPixels. TODO: fix this by blitting
  // the FBO to a non-multisampled FBO and reading from that.
  /* format.setSamples(4); */

  // These don't seem to be necessary. Defaults?
  /* format.setRedBufferSize(8); */
  /* format.setGreenBufferSize(8); */
  /* format.setBlueBufferSize(8); */
  /* format.setAlphaBufferSize(8); */

  setFormat(format);

  connect(&auto_rotate_timer, &QTimer::timeout, [=]() {
    makeCurrent();
    renderer.autoRotate();
    update();
  });
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::paintGL() {
  renderer.render();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::initializeGL() {
  initializeOpenGLFunctions();
  renderer.initializeGL();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::resizeGL(int width, int height) {
  renderer.resize(width, height);
  update();
}

/* ------------------------------------------------------------------------- */

MadeupRenderer &MadeupCanvas::getRenderer() {
  return renderer;
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mousePressEvent(QMouseEvent *event) {
  makeCurrent();
  auto_rotate_timer.stop();
  renderer.leftMouseDownAt(event->x(), height() - event->y());
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mouseMoveEvent(QMouseEvent *event) {
  makeCurrent();
  renderer.leftMouseDraggedTo(event->x(), height() - event->y());
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mouseReleaseEvent(QMouseEvent *event) {
  makeCurrent();
  float inertia = renderer.leftMouseUpAt(event->x(), height() - event->y());
  if (inertia > 3) {
    auto_rotate_timer.start(33);
  }
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::wheelEvent(QWheelEvent *event) {
  makeCurrent();
  renderer.scroll(event->angleDelta().y());
  update();
}

/* ------------------------------------------------------------------------- */

