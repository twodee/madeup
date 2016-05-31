#include "MadeupCanvas.h"
#include <iostream>

using std::string;

/* ------------------------------------------------------------------------- */

MadeupCanvas::MadeupCanvas(QWidget *parent) :
  QOpenGLWidget(parent),
  renderer(new MadeupRenderer()) {
  QSurfaceFormat format;
  format.setVersion(3, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  setFormat(format);
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::paintGL() {
  renderer->Render();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::initializeGL() {
  initializeOpenGLFunctions();
  renderer->InitializeGL();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::setTrimesh(td::Trimesh *trimesh) {
  renderer->SetTrimesh(trimesh);
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::resizeGL(int width, int height) {
  renderer->Resize(width, height);
  update();
}

/* ------------------------------------------------------------------------- */

MadeupRenderer *MadeupCanvas::getRenderer() {
  return renderer;
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mousePressEvent(QMouseEvent *event) {
  /* makeCurrent(); */
  renderer->LeftMouseDownAt(event->x(), height() - event->y());
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mouseMoveEvent(QMouseEvent *event) {
  /* makeCurrent(); */
  renderer->LeftMouseDraggedTo(event->x(), height() - event->y());
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mouseReleaseEvent(QMouseEvent *event) {
  /* makeCurrent(); */
  renderer->LeftMouseUpAt(event->x(), height() - event->y());
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::wheelEvent(QWheelEvent *event) {
  renderer->Scroll(event->angleDelta().y());
  update();
}

/* ------------------------------------------------------------------------- */

