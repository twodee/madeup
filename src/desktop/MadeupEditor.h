#ifndef MADEUPEDITOR_H
#define MADEUPEDITOR_H

#include <QKeyEvent>
#include <QWidget>
#include <QPlainTextEdit>

class MadeupEditor : public QPlainTextEdit {
  public:
    MadeupEditor(QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *event);

  private:

};

#endif
