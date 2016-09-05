#ifndef MADEUPCANVAS_H
#define MADEUPCANVAS_H

/* #define GL_GLEXT_PROTOTYPES */
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Compatibility>
#include <QTimer>

#include "MadeupRenderer.h"
#include "madeup/Environment.h"
#include "twodee/Trimesh.h"

class MadeupCanvas : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Compatibility {
  public:
    explicit MadeupCanvas(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    MadeupRenderer &getRenderer();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

  private:
    QTimer auto_rotate_timer;
    MadeupRenderer renderer;
};

#endif // CANVAS_H
