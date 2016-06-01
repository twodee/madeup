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
    void onRun();
    void onTextChanged();
    void selectBackgroundColor();
    void selectFont();
    void onFit();

  private:
    QTextEdit *editor;
    QTextEdit *console;
    MadeupCanvas *canvas;
};

/* ------------------------------------------------------------------------- */

#endif // MUPPER_WINDOW_H
