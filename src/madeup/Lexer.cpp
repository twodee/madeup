#include <iostream>
#include <sstream>

#include "madeup/Lexer.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

Lexer::Lexer(std::istream &in) :
  in(in),
  location() {
  if (!in.good()) {
    throw MessagedException("bad input stream");
  }
}


const std::vector<Token> &Lexer::getTokens() const {
  return tokens;
}

/* ------------------------------------------------------------------------- */

const std::vector<Token> &Lexer::lex() {
  location.start_row = 1;
  location.end_row = 1;
  location.end_column = -1;
  location.start_index = 0;
  location.end_index = -1;

  tokens.clear();

  bool isEOF;
  do {
    Token token = getToken();
    tokens.push_back(token);
    isEOF = token.getType() == Token::END_OF_FILE;
  } while (!isEOF);

  if (tokens.size() == 1 || tokens[tokens.size() - 2].getType() != Token::NEWLINE) {
    Token eof = tokens.back();
    tokens.pop_back();
    tokens.push_back(Token(Token::NEWLINE, "\n", location));
    tokens.push_back(eof);
  }

  return tokens;
}

/* ------------------------------------------------------------------------- */

Token Lexer::makeToken(Token::token_t type) {
  return Token(type, text_so_far, location);
}

/* ------------------------------------------------------------------------- */

Token Lexer::getToken() {
  text_so_far = "";

  if (in.eof()) {
    return Token(Token::END_OF_FILE, "END OF FILE", location);
  }

  // This token picks up where the last one left off, skipping any whitespace.
  // Start will drop anchor here.
  location.start_column = location.end_column + 1;
  location.start_index = location.end_index + 1;

  int c = in.get();
  while (isspace(c) && c != '\n') {
    ++location.start_column;
    ++location.start_index;
    c = in.get();
  }

  // But end will get carried along the token's extent.
  location.end_column = location.start_column;
  location.end_index = location.start_index;

  if (c == EOF) {
    return Token(Token::END_OF_FILE, "END OF FILE", location);
  } else {
    text_so_far += (char) c;
  }

  if (c == '(') {
    return makeToken(Token::LEFT_PARENTHESIS);
  } else if (c == ')') {
    return makeToken(Token::RIGHT_PARENTHESIS);
  } else if (c == '^') {
    return makeToken(Token::CIRCUMFLEX);
  } else if (c == '+') {
    return makeToken(Token::PLUS);
  } else if (c == '*') {
    return makeToken(Token::TIMES);
  } else if (c == '%') {
    return makeToken(Token::REMAINDER);
  } else if (c == '|') {
    return makeToken(Token::PIPE);
  } else if (c == ',') {
    return makeToken(Token::COMMA);
  } else if (c == '[') {
    return makeToken(Token::LEFT_BRACKET);
  } else if (c == ']') {
    return makeToken(Token::RIGHT_BRACKET);
  } else if (c == ':') {
    return makeToken(Token::COLON);
  } else if (c == '\n') {
    Token token = makeToken(Token::NEWLINE);
    ++location.start_row;
    ++location.end_row;
    location.end_column = -1;
    return token;
  } else if (c == '/') {
    return getTokenAfterSlash();
  } else if (c == '.') {
    return getTokenAfterDot();
  } else if (c == '-') {
    return getTokenAfterMinus();
  } else if (c == '<') {
    return getTokenAfterLessThan();
  } else if (c == '>') {
    return getTokenAfterGreaterThan();
  } else if (c == '=') {
    return getTokenAfterEquals();
  } else if (c == '!') {
    return getTokenAfterBang();
  } else if (c == '"') {
    return getTokenAfterQuote();
  } else if (isalpha(c)) {
    return getTokenAfterLetter();
  } else if (isdigit(c)) {
    return getTokenAfterDigit();
  } else {
    std::stringstream ss;
    ss << location.toAnchor() << ": I found a character that I didn't recognize: ";
    if (isprint(c)) {
      ss << (char) c;
    } else {
      ss << std::hex << std::uppercase << (int) c << std::dec << std::nouppercase;
    }
    ss << ".";
    throw MessagedException(ss.str());
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterSlash() {
  int c = in.get();
  if (c == '/') {
    ++location.end_column;
    ++location.end_index;
    text_so_far += (char) c;
    return makeToken(Token::REAL_DIVIDE);
  } else {
    in.putback(c);
    return makeToken(Token::DIVIDE);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterDot() {
  int c = in.get();
  if (c == '.') {
    ++location.end_column;
    ++location.end_index;
    text_so_far += (char) c;
    return makeToken(Token::RANGE);
  } else {
    in.putback(c);
    return getTokenAfterLetter();
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterMinus() {
  int c = in.get();
  if (c == '-') {
    do {
      ++location.end_column;
      ++location.end_index;
      c = in.get();
    } while (c != EOF && c != '\n');
    in.putback(c);
    return getToken();
  } else if (isdigit(c)) {
    ++location.end_column;
    ++location.end_index;
    text_so_far += (char) c;
    return getTokenAfterDigit();
  } else {
    in.putback(c);
    return makeToken(Token::MINUS);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterQuote() {
  text_so_far = "";

  int c = in.get();
  ++location.end_column;
  ++location.end_index;

  while (c != EOF && c != '"' && c != '\n') {
    text_so_far += (char) c;
    c = in.get();
    ++location.end_column;
    ++location.end_index;
  }

  if (c != '"') {
    --location.end_column;
    --location.end_index;
    in.putback(c);
    std::stringstream ss;
    ss << location.toAnchor() << ": I was reading the string \"" << text_so_far << "\", but it didn't end with a quotation mark.";
    throw MessagedException(ss.str());
  }

  return Token(Token::STRING, text_so_far, location);
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterLetter() {
  int c = in.get();

  while (c != EOF && (isalpha(c) || isdigit(c) || (c == '.' && in.peek() != '.'))) {
    ++location.end_column;
    ++location.end_index;
    text_so_far += (char) c;
    c = in.get();
  }
  in.putback(c);

  if (text_so_far == "and") {
    return makeToken(Token::AND);
  } else if (text_so_far == "by") {
    return makeToken(Token::BY);
  } else if (text_so_far == "else") {
    return makeToken(Token::ELSE);
  } else if (text_so_far == "end") {
    return makeToken(Token::END);
  } else if (text_so_far == "false") {
    return makeToken(Token::FALSE);
  } else if (text_so_far == "for") {
    return makeToken(Token::FOR);
  } else if (text_so_far == "if") {
    return makeToken(Token::IF);
  } else if (text_so_far == "in") {
    return makeToken(Token::IN);
  } else if (text_so_far == "not") {
    return makeToken(Token::NOT);
  } else if (text_so_far == "nothing") {
    return makeToken(Token::NOTHING);
  } else if (text_so_far == "of") {
    return makeToken(Token::OF);
  } else if (text_so_far == "or") {
    return makeToken(Token::OR);
  } else if (text_so_far == "repeat") {
    return makeToken(Token::REPEAT);
  } else if (text_so_far == "repeatwich") {
    return makeToken(Token::REPEATWICH);
  } else if (text_so_far == "then") {
    return makeToken(Token::THEN);
  } else if (text_so_far == "around") {
    return makeToken(Token::AROUND);
  } else if (text_so_far == "through") {
    return makeToken(Token::THROUGH);
  } else if (text_so_far == "to") {
    return makeToken(Token::TO);
  } else if (text_so_far == "true") {
    return makeToken(Token::TRUE);
  } else if (text_so_far == "while") {
    return makeToken(Token::WHILE);
  } else {
    return makeToken(Token::ID);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterDigit() {
  int c = in.get();
  ++location.end_column;
  ++location.end_index;

  while (c != EOF && isdigit(c)) {
    text_so_far += (char) c;
    c = in.get();
    ++location.end_column;
    ++location.end_index;
  }

  if (isalpha(c)) {
    std::stringstream ss;
    ss << location.toAnchor() << ": I was reading the integer " << text_so_far << ", when suddenly I hit '" << (char) c << "'. What do I do with that?";
    throw MessagedException(ss.str());
  }

  if (c == EOF || c != '.' || (c == '.' && in.peek() == '.')) {
    in.putback(c);
    --location.end_column;
    --location.end_index;
    return makeToken(Token::INTEGER);
  } else {
    text_so_far += (char) c;

    c = in.get();
    ++location.end_column;
    ++location.end_index;

    // Must be at least one number after decimal.
    if (!isdigit(c)) {
      std::stringstream ss;
      ss << location.toAnchor() << ": I was reading the number " << text_so_far.substr(0, text_so_far.length() - 1) << ", but I couldn't find any digits after the decimal point. You must have at least one digit there.";
      throw MessagedException(ss.str());
    }
   
    while (c != EOF && isdigit(c)) {
      text_so_far += (char) c;
      c = in.get();
      ++location.end_column;
      ++location.end_index;
    }

    if (isalpha(c)) {
      std::stringstream ss;
      ss << location.toAnchor() << ": I was reading the number " << text_so_far << ", when suddenly I hit a '" << (char) c << "'.";
      throw MessagedException(ss.str());
    }

    in.putback(c);
    --location.end_column;
    --location.end_index;
    return makeToken(Token::REAL);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterLessThan() {
  int c = in.get();
  if (c == '=') {
    ++location.end_column;
    ++location.end_index;
    text_so_far += (char) c;
    return makeToken(Token::LESS_THAN_OR_EQUAL_TO);
  } else {
    in.putback(c);
    return makeToken(Token::LESS_THAN);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterGreaterThan() {
  int c = in.get();
  if (c == '=') {
    ++location.end_column;
    ++location.end_index;
    text_so_far += (char) c;
    return makeToken(Token::GREATER_THAN_OR_EQUAL_TO);
  } else {
    in.putback(c);
    return makeToken(Token::GREATER_THAN);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterEquals() {
  int c = in.get();
  if (c == '=') {
    ++location.end_column;
    ++location.end_index;
    text_so_far += (char) c;
    return makeToken(Token::EQUAL_TO);
  } else {
    in.putback(c);
    return makeToken(Token::ASSIGN);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterBang() {
  int c = in.get();

  if (c == '=') {
    ++location.end_column;
    ++location.end_index;
    text_so_far += (char) c;
    return makeToken(Token::NOT_EQUAL_TO);
  } else {
    std::stringstream ss;
    ss << location.toAnchor() << ": I ran into " << text_so_far << " and I didn't know what to do with it. It's not part of the language I speak.";
    throw MessagedException(ss.str());
  }
}

/* ------------------------------------------------------------------------- */

}
