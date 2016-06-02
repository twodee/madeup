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
  renderer->render();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::initializeGL() {
  initializeOpenGLFunctions();
  renderer->initializeGL();
}

/* ------------------------------------------------------------------------- */

/* void MadeupCanvas::setTrimesh(td::Trimesh *trimesh) { */
  /* renderer->setTrimesh(trimesh); */
  /* update(); */
/* } */

/* ------------------------------------------------------------------------- */

void MadeupCanvas::resizeGL(int width, int height) {
  renderer->resize(width, height);
  update();
}

/* ------------------------------------------------------------------------- */

MadeupRenderer *MadeupCanvas::getRenderer() {
  return renderer;
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mousePressEvent(QMouseEvent *event) {
  /* makeCurrent(); */
  renderer->leftMouseDownAt(event->x(), height() - event->y());
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mouseMoveEvent(QMouseEvent *event) {
  /* makeCurrent(); */
  renderer->leftMouseDraggedTo(event->x(), height() - event->y());
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::mouseReleaseEvent(QMouseEvent *event) {
  /* makeCurrent(); */
  renderer->leftMouseUpAt(event->x(), height() - event->y());
  update();
}

/* ------------------------------------------------------------------------- */

void MadeupCanvas::wheelEvent(QWheelEvent *event) {
  renderer->scroll(event->angleDelta().y());
  update();
}

/* ------------------------------------------------------------------------- */

/* void MadeupCanvas::setPaths(const std::vector<std::vector<madeup::Turtle> > &paths) { */
  /* renderer->setPaths(paths); */
  /* update(); */
/* } */

/* ------------------------------------------------------------------------- */

/* void MadeupCanvas::setPathStrokeWidth(float width) { */
  /* renderer->setPathStrokeWidth(width); */
  /* update(); */
/* } */

/* ------------------------------------------------------------------------- */

