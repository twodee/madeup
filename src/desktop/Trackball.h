#ifndef TRACKBALL_H
#define TRACKBALL_H

#include "twodee/QVector2.h"
#include "twodee/QVector3.h"
#include "twodee/QMatrix4.h"

namespace td {

/* ------------------------------------------------------------------------- */

class Trackball {
  public:
    Trackball();

    void Reset();
    void Rotate(float angle, const QVector3<float>& axis);
    void Autorotate();
    const QMatrix4<float>& GetMatrix() const;
    void Start(int x, int y);
    void Drag(int x, int y, float factor = 1.0f);
    bool Stop();
    void SetViewport(int width, int height, int x = 0, int y = 0);

    const QVector2<int> &GetDelta() const;

  private:
    td::QMatrix4<float> to_rotation;

    td::QVector2<int> mouse_at;
    td::QVector2<int> mouse_delta;

    td::QMatrix4<float> last_rotation;

    int x;
    int y;
    int width;
    int height;
};

/* ------------------------------------------------------------------------- */

}

#endif
