#include "MadeupEditor.h"

/* ------------------------------------------------------------------------- */

MadeupEditor::MadeupEditor(QWidget *parent) :
  QPlainTextEdit(parent) {
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Tab) {
    insertPlainText("  ");
  } else {
    QPlainTextEdit::keyPressEvent(event);
  }
}

/* ------------------------------------------------------------------------- */

