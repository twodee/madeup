#ifndef MUPPER_WINDOW_H
#define MUPPER_WINDOW_H

#include <QMainWindow>
#include <QTextEdit>

#include "MadeupCanvas.h"

/* ------------------------------------------------------------------------- */

namespace Ui {
  class MupperWindow;
}

/* ------------------------------------------------------------------------- */

class MupperWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit MupperWindow(QWidget *parent = 0);
    ~MupperWindow();

  public slots:
    void onSolidify();
    void onPathify();
    void onTextChanged();
    void selectBackgroundColor();
    void selectPathColor();
    void selectVertexColor();
    void selectFont();
    void onFit();

  private:
    void selectColor(const td::QVector4<float> &initial_color,
                     std::function<void(const QColor &)> onSelect);
    void onRun(madeup::GeometryMode::geometry_mode_t mode);

    QTextEdit *editor;
    QTextEdit *console;
    MadeupCanvas *canvas;
};

/* ------------------------------------------------------------------------- */

#endif // MUPPER_WINDOW_H
