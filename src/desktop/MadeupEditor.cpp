#include <iostream>
#include <sstream>

#include <QAction>
#include <QUndoCommand>

#include "MadeupEditor.h"

#include "madeup/Lexer.h"
#include "twodee/MessagedException.h"

using namespace madeup;

class Transition : public QUndoCommand {
  public:
    Transition(MadeupEditor *editor,
               const QString &previous_text,
               const QTextCursor &previous_cursor,
               const QString &current_text,
               const QTextCursor &current_cursor) :
      editor(editor),
      previous_text(previous_text),
      previous_cursor(previous_cursor),
      current_text(current_text),
      current_cursor(current_cursor) {
      setText("edit text");
    }

    virtual void undo() {
      editor->blockSignals(true);

      int start = previous_cursor.selectionStart();
      int end = previous_cursor.selectionEnd();
      editor->document()->setPlainText(current_text); // resets cursor selection

      QTextCursor cursor = editor->textCursor();
      cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, start);
      cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, (end - start));
      editor->setTextCursor(cursor);

      editor->blockSignals(false);
      editor->hilite();
    }

    virtual void redo() {
      editor->blockSignals(true);

      int start = current_cursor.selectionStart();
      int end = current_cursor.selectionEnd();
      editor->document()->setPlainText(current_text); // resets cursor selection

      QTextCursor cursor = editor->textCursor();
      cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, start);
      cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, (end - start));
      editor->setTextCursor(cursor);

      editor->blockSignals(false);
      editor->hilite();
    }

  private:
    MadeupEditor *editor;
    QString previous_text;
    QTextCursor previous_cursor;
    QString current_text;
    QTextCursor current_cursor;
};

/* ------------------------------------------------------------------------- */

MadeupEditor::MadeupEditor(QWidget *parent) :
  QPlainTextEdit(parent),
  undo_stack(new QUndoStack(this)),
  previous_text(),
  previous_cursor(textCursor()) {
  setUndoRedoEnabled(false);
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Tab) {
    insertPlainText("  ");
  } else if (event->key() == Qt::Key_Z && event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier)) {
    redo();
  } else if (event->key() == Qt::Key_Z && event->modifiers().testFlag(Qt::ControlModifier)) {
    undo();
  } else {
    QPlainTextEdit::keyPressEvent(event);
  }
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::undo() {
  undo_stack->undo(); 
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::redo() {
  undo_stack->redo();  
}

/* ------------------------------------------------------------------------- */

QUndoStack *MadeupEditor::getUndoStack() const {
  return undo_stack; 
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::onTextChanged() {
  const QString &current_text = toPlainText();
  const QTextCursor &current_cursor = textCursor();

  undo_stack->push(new Transition(this, previous_text, previous_cursor, current_text, current_cursor));
  previous_text = current_text;
  previous_cursor = current_cursor;
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::hilite() {
  blockSignals(true);

  std::string source = toPlainText().toStdString();
  std::stringstream in(source);
  Lexer lexer(in);
  lexer.keepComments(true);

  try {
    lexer.lex();
  } catch (td::MessagedException e) {
    std::cerr << "Uh oh! " << e.GetMessage() << std::endl;
  }

  const std::vector<Token> &tokens = lexer.getTokens();
  
  QTextCharFormat punctuation_format;
  punctuation_format.setForeground(Qt::blue);

  QTextCharFormat number_format;
  number_format.setForeground(QColor("#CF6A4C"));

  QTextCharFormat operator_format;
  operator_format.setForeground(QColor("#CDA869"));

  QTextCharFormat keyword_format;
  keyword_format.setForeground(QColor("#CDA869"));

  QTextCharFormat comment_format;
  comment_format.setForeground(QColor("#5F5A60"));

  QTextCharFormat identifier_format;
  identifier_format.setForeground(QColor("#7587A6"));

  QTextCharFormat string_format;
  string_format.setForeground(QColor("#8F9D6A"));

  QTextCharFormat comma_format;
  comma_format.setForeground(QColor("#FFFFFF"));

  QTextCursor cursor(document());

  // We skip the last two tokens (NEWLINE and END_OF_FILE) because they aren't
  // really in the original source.
  unsigned int ti = 0;
  for (auto token = tokens.begin(); ti + 2 < tokens.size(); ++token, ++ti) {
    SourceLocation location = token->getLocation();
    cursor.setPosition(location.getStartIndex(), QTextCursor::MoveAnchor);
    cursor.setPosition(location.getEndIndex() + 1, QTextCursor::KeepAnchor);

    switch (token->getType()) {
      case Token::COMMA:
      case Token::LEFT_PARENTHESIS:
      case Token::RIGHT_PARENTHESIS:
      case Token::LEFT_CURLY_BRACE:
      case Token::RIGHT_CURLY_BRACE:
      case Token::LEFT_BRACKET:
      case Token::RIGHT_BRACKET:
      case Token::AT_SIGN:
      case Token::UNKNOWN:
        cursor.setCharFormat(comma_format);
        break;
      case Token::INTEGER:
      case Token::REAL:
      case Token::TRUE:
      case Token::FALSE:
      case Token::NOTHING:
        cursor.setCharFormat(number_format);
        break;
      case Token::PLUS:
      case Token::COLON:
      case Token::MINUS:
      case Token::TIMES:
      case Token::DIVIDE:
      case Token::REAL_DIVIDE:
      case Token::CIRCUMFLEX:
      case Token::LESS_THAN:
      case Token::LESS_THAN_OR_EQUAL_TO:
      case Token::GREATER_THAN:
      case Token::GREATER_THAN_OR_EQUAL_TO:
      case Token::EQUAL_TO:
      case Token::NOT_EQUAL_TO:
      case Token::REMAINDER:
      case Token::ASSIGN:
      case Token::PIPE:
      case Token::RANGE:
        cursor.setCharFormat(operator_format);
        break;
      case Token::AND:
      case Token::IN:
      case Token::AROUND:
      case Token::THROUGH:
      case Token::OR:
      case Token::NOT:
      case Token::FOR:
      case Token::OF:
      case Token::BY:
      case Token::ELSE:
      case Token::IF:
      case Token::REPEAT:
      case Token::END:
      case Token::THEN:
      case Token::WHILE:
      case Token::TO:
        cursor.setCharFormat(keyword_format);
        break;
      case Token::COMMENT:
        cursor.setCharFormat(comment_format);
        break;
      case Token::ID:
        cursor.setCharFormat(identifier_format);
        break;
      case Token::STRING:
        cursor.setCharFormat(string_format);
        break;
      case Token::NEWLINE:
      case Token::END_OF_FILE:
        break;
    }
  }

  blockSignals(false);
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::indent() {
  blockSignals(true);   

  QTextCursor cursor = textCursor();
  QTextCursor before_cursor = cursor;

  int start = cursor.selectionStart();
  int end = cursor.selectionEnd();

  // Go to start position.
  cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, start);
  cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);

  // Walk through and indent each line.
  int ninserted = 0;
  bool is_moved = true;
  while (is_moved && cursor.position() <= end + ninserted) {
    cursor.insertText("  ");
    ninserted += 2;
    is_moved = cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
  }

  setTextCursor(before_cursor);

  blockSignals(false);   
  onTextChanged();
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::unindent() {
  blockSignals(true);   

  QTextCursor cursor = textCursor();
  QTextCursor before_cursor = cursor;

  int start = cursor.selectionStart();
  int end = cursor.selectionEnd();

  // Go to start position.
  cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, start);
  cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);

  // Walk through and indent each line.
  int ninserted = 0;
  bool is_moved = true;
  while (is_moved && cursor.position() <= end + ninserted) {
    if (document()->characterAt(cursor.position()) == ' ') {
      cursor.deleteChar();
      ninserted -= 1;
    }
    if (document()->characterAt(cursor.position()) == ' ') {
      cursor.deleteChar();
      ninserted -= 1;
    }
    is_moved = cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
  }

  setTextCursor(before_cursor);

  blockSignals(false);   
  onTextChanged();
}

/* ------------------------------------------------------------------------- */

void MadeupEditor::comment() {
  blockSignals(true);   

  QTextCursor cursor = textCursor();
  QTextCursor before_cursor = cursor;

  int start = cursor.selectionStart();
  int end = cursor.selectionEnd();

  // Go to start position.
  cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
  cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, start);
  cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);

  bool is_adding = document()->characterAt(cursor.position()) != '-' ||
                   document()->characterAt(cursor.position() + 1) != '-';

  // Walk through and indent each line.
  int ninserted = 0;
  bool is_moved = true;
  while (is_moved && cursor.position() <= end + ninserted) {
    if (is_adding) {
      cursor.insertText("-- ");
      ninserted += 3;
    } else {
      if (document()->characterAt(cursor.position()) == '-' &&
          document()->characterAt(cursor.position() + 1) == '-') {
        cursor.deleteChar();
        cursor.deleteChar();
        ninserted -= 2;
        if (document()->characterAt(cursor.position()) == ' ') {
          cursor.deleteChar();
          ninserted -= 1;
        }
      }
    }
    is_moved = cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
  }

  setTextCursor(before_cursor);

  blockSignals(false);   
  onTextChanged();
}

/* ------------------------------------------------------------------------- */

