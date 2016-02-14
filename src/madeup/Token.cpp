#include <iostream>

#include "madeup/Token.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

Token::Token(token_t type,
             const std::string &text,
             const SourceLocation &location) :
  type(type),
  text(text),
  location(location) {
}

/* ------------------------------------------------------------------------- */

Token::token_t Token::getType() const {
  return type;
}

/* ------------------------------------------------------------------------- */

const std::string Token::getText() const {
  return text;
}

/* ------------------------------------------------------------------------- */

const std::string Token::getQuotedText() const {
  if (type == NEWLINE) {
    return "a linebreak";
  } else if (type == END_OF_FILE) {
    return "the end of the file";
  } else {
    return "'" + getText() + "'";
  }
}

/* ------------------------------------------------------------------------- */

const SourceLocation &Token::getLocation() const {
  return location;
}

/* ------------------------------------------------------------------------- */

const std::string Token::getTypeString() const {
  switch (type) {
    case COMMENT:
      return "COMMENT";
    case LEFT_PARENTHESIS:
      return "LEFT_PARENTHESIS";
    case RIGHT_PARENTHESIS:
      return "RIGHT_PARENTHESIS";
    case COLON:
      return "COLON";
    case NOTHING:
      return "NOTHING";
    case CIRCUMFLEX:
      return "CIRCUMFLEX";
    case AROUND:
      return "AROUND";
    case ID:
      return "ID";
    case LEFT_BRACKET:
      return "LEFT_BRACKET";
    case RIGHT_BRACKET:
      return "RIGHT_BRACKET";
    case LEFT_CURLY_BRACE:
      return "LEFT_CURLY_BRACE";
    case RIGHT_CURLY_BRACE:
      return "RIGHT_CURLY_BRACE";
    case PLUS:
      return "PLUS";
    case MINUS:
      return "MINUS";
    case TIMES:
      return "TIMES";
    case DIVIDE:
      return "DIVIDE";
    case REAL_DIVIDE:
      return "REAL_DIVIDE";
    case INTEGER:
      return "INTEGER";
    case REAL:
      return "REAL";
    case STRING:
      return "STRING";
    case COMMA:
      return "COMMA";
    case LESS_THAN:
      return "LESS_THAN";
    case LESS_THAN_OR_EQUAL_TO:
      return "LESS_THAN_OR_EQUAL_TO";
    case GREATER_THAN:
      return "GREATER_THAN";
    case GREATER_THAN_OR_EQUAL_TO:
      return "GREATER_THAN_OR_EQUAL_TO";
    case EQUAL_TO:
      return "EQUAL_TO";
    case NOT_EQUAL_TO:
      return "NOT_EQUAL_TO";
    case REMAINDER:
      return "REMAINDER";
    case ASSIGN:
      return "ASSIGN";
    case PIPE:
      return "PIPE";
    case RANGE:
      return "RANGE";
    case NEWLINE:
      return "NEWLINE";
    case AND:
      return "AND";
    case BY:
      return "BY";
    case ELSE:
      return "ELSE";
    case END:
      return "END";
    case FALSE:
      return "FALSE";
    case FOR:
      return "FOR";
    case IF:
      return "IF";
    case IN:
      return "IN";
    case NOT:
      return "NOT";
    case OF:
      return "OF";
    case OR:
      return "OR";
    case REPEAT:
      return "REPEAT";
    case THEN:
      return "THEN";
    case THROUGH:
      return "THROUGH";
    case TO:
      return "TO";
    case TRUE:
      return "TRUE";
    case WHILE:
      return "WHILE";
    case END_OF_FILE:
      return "END_OF_FILE";
    default:
      return "???";
  }
}

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const Token &token) {
  std::string text = token.getType() == Token::NEWLINE ? "\\n" : token.getText();
  out << token.getLocation().getStartRow()
      << "(" << token.getLocation().getStartIndex() << "-" << token.getLocation().getEndIndex() << ")"
      << " " << token.getTypeString() << ": " << text;
  return out;
}

/* ------------------------------------------------------------------------- */

}

