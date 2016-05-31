#ifndef MUPPER_WINDOW_H
#define MUPPER_WINDOW_H

#include <QMainWindow>

#include "ui_mupper_window.h"
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
    void onRun();
    void onTextChanged();
    void setBackgroundColor();
    void onFit();

  private:
    Ui::MupperWindow *ui;
    QTextEdit *editor;
    MadeupCanvas *canvas;
};

/* ------------------------------------------------------------------------- */

#endif // MUPPER_WINDOW_H
