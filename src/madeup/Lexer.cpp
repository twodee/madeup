#include <iostream>
#include <sstream>

#include "madeup/Lexer.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

Lexer::Lexer(std::istream &in) :
  in(in) {
  if (!in.good()) {
    throw MessagedException("bad input stream");
  }
}

/* ------------------------------------------------------------------------- */

const std::vector<Token> &Lexer::getTokens() const {
  return tokens;
}

/* ------------------------------------------------------------------------- */

const std::vector<Token> &Lexer::lex() {
  start_row = 0;
  end_row = 0;
  end_column = -1;
  tokens.clear();

  bool isEOF;
  do {
    Token token = getToken();
    tokens.push_back(token);
    isEOF = token.getType() == Token::END_OF_FILE;
  } while (!isEOF);

  return tokens;
}

/* ------------------------------------------------------------------------- */

Token Lexer::makeToken(Token::token_t type) {
  return Token(type, text_so_far, start_row, start_column, end_row, end_column);
}

/* ------------------------------------------------------------------------- */

Token Lexer::getToken() {
  text_so_far = "";
  
  /* std::cout << "start_row: " << start_row << std::endl; */
  /* std::cout << "start_column: " << start_column << std::endl; */
  /* std::cout << "end_row: " << end_row << std::endl; */
  /* std::cout << "end_column: " << end_column << std::endl; */

  if (in.eof()) {
    return makeToken(Token::END_OF_FILE);
  }

  start_column = end_column + 1;
  int c = in.get();

  // Skip overwhitespace.
  while (isspace(c) && c != '\n') {
    ++start_column;
    c = in.get();
  }

  end_column = start_column;

  if (in.eof()) {
    return makeToken(Token::END_OF_FILE);
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
    ++start_row;
    ++end_row;
    end_column = -1;
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
  }

  std::stringstream ss;
  ss << start_row << "(" << start_column << "-" << end_column << "): I found a character that I didn't recognize: " << (char) c << ".";
  throw MessagedException(ss.str());
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterSlash() {
  int c = in.get();
  if (c == '/') {
    ++end_column;
    text_so_far += (char) c;
    return makeToken(Token::REAL_DIVIDE);
  } else {
    --end_column;
    in.putback(c);
    return makeToken(Token::DIVIDE);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterDot() {
  int c = in.get();
  if (c == '.') {
    ++end_column;
    text_so_far += (char) c;
    return makeToken(Token::RANGE);
  } else {
    --end_column;
    in.putback(c);
    return makeToken(Token::DOT);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterMinus() {
  int c = in.get();
  if (c == '-') {
    do {
      ++end_column;
      c = in.get();
    } while (!in.eof() && c != '\n');
    in.putback(c);
    return getToken();
  } else if (isdigit(c)) {
    ++end_column;
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
  ++end_column;

  while (!in.eof() && c != '"' && c != '\n') {
    text_so_far += (char) c;
    c = in.get();
    ++end_column;
  }

  if (c != '"') {
    --end_column;
    in.putback(c);
    std::stringstream ss;
    ss << start_row << "(" << start_column << "-" << end_column << "): I was reading the string \"" << text_so_far << "\", but it didn't end with a quotation mark.";
    throw MessagedException(ss.str());
  }

  return Token(Token::STRING, text_so_far, start_row, start_column, end_row, end_column);
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterLetter() {
  int c = in.get();

  while (!in.eof() && (isalpha(c) || isdigit(c))) {
    ++end_column;
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
  } else if (text_so_far == "of") {
    return makeToken(Token::OF);
  } else if (text_so_far == "or") {
    return makeToken(Token::OR);
  } else if (text_so_far == "repeat") {
    return makeToken(Token::REPEAT);
  } else if (text_so_far == "then") {
    return makeToken(Token::THEN);
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
  ++end_column;

  while (!in.eof() && isdigit(c)) {
    text_so_far += (char) c;
    c = in.get();
    ++end_column;
  }

  if (isalpha(c)) {
    std::stringstream ss;
    ss << start_row << "(" << start_column << "-" << end_column << "): I was reading the integer " << text_so_far << ", when suddenly I hit '" << (char) c << "'. What do I do with that?";
    throw MessagedException(ss.str());
  }

  if (in.eof() || c != '.' || (c == '.' && in.peek() == '.')) {
    in.putback(c);
    --end_column;
    return makeToken(Token::INTEGER);
  } else {
    text_so_far += (char) c;

    c = in.get();
    ++end_column;

    // Must be at least one number after decimal.
    if (!isdigit(c)) {
      // TODO strip off decimal?
      std::stringstream ss;
      ss << start_row << "(" << start_column << "-" << end_column << "): I was reading the number " << text_so_far.substr(0, text_so_far.length() - 1) << ", but I couldn't find any digits after the decimal point. You must have at least one digit there.";
      throw MessagedException(ss.str());
    }
   
    while (!in.eof() && isdigit(c)) {
      text_so_far += (char) c;
      c = in.get();
      ++end_column;
    }

    if (isalpha(c)) {
      std::stringstream ss;
      ss << start_row << "(" << start_column << "-" << end_column << "): I was reading the number " << text_so_far << ", when suddenly I hit a '" << (char) c << "'.";
      throw MessagedException(ss.str());
    }

    in.putback(c);
    --end_column;
    return makeToken(Token::REAL);
  }
}

/* ------------------------------------------------------------------------- */

Token Lexer::getTokenAfterLessThan() {
  int c = in.get();
  if (c == '=') {
    ++end_column;
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
    ++end_column;
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
    ++end_column;
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
    ++end_column;
    text_so_far += (char) c;
    return makeToken(Token::NOT_EQUAL_TO);
  } else {
    std::stringstream ss;
    ss << start_row << "(" << start_column << "-" << end_column << "): I ran into " << text_so_far << " and I didn't know what to do with it. It's not part of the language I speak.";
    throw MessagedException(ss.str());
  }
}

/* ------------------------------------------------------------------------- */

}
