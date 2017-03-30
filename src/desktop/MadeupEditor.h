#ifndef MADEUPEDITOR_H
#define MADEUPEDITOR_H

#include <QKeyEvent>
#include <QWidget>
#include <QPlainTextEdit>
#include <QUndoStack>

class MadeupEditor : public QPlainTextEdit {
  public:
    MadeupEditor(QWidget *parent = nullptr);

    QUndoStack *getUndoStack() const;
    void keyPressEvent(QKeyEvent *event);
    void undo();
    void redo();
    void onTextChanged();
    void hilite();

  private:
    QUndoStack *undo_stack;
    QString previous_text;
};

#endif
