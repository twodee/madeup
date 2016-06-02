#ifndef MADEUPCANVAS_H
#define MADEUPCANVAS_H

#define GL_GLEXT_PROTOTYPES
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>

#include "MadeupRenderer.h"
#include "madeup/Environment.h"
#include "twodee/Trimesh.h"

class MadeupCanvas : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core {
  public:
    explicit MadeupCanvas(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    /* void setTrimesh(td::Trimesh *trimesh); */
    /* void setPaths(const std::vector<std::vector<madeup::Turtle> > &paths); */
    /* void setPathStrokeWidth(float width); */
    MadeupRenderer *getRenderer();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

  private:
    MadeupRenderer *renderer;
};

#endif // CANVAS_H
