#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>
#include <string>

#include "madeup/Token.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class Lexer {
  public:
    Lexer(std::istream &in);
    const std::vector<Token> &getTokens() const;
    const std::vector<Token> &lex();

  private:
    Token makeToken(Token::token_t type);
    Token getToken();
    Token getTokenAfterSlash();
    Token getTokenAfterDot();
    Token getTokenAfterMinus();
    Token getTokenAfterQuote();
    Token getTokenAfterLetter();
    Token getTokenAfterDigit();
    Token getTokenAfterLessThan();
    Token getTokenAfterGreaterThan();
    Token getTokenAfterEquals();
    Token getTokenAfterBang();

    std::vector<Token> tokens;
    std::istream &in;
    int start_row;
    int end_row;
    int start_column;
    int end_column;
    std::string text_so_far;
};

/* ------------------------------------------------------------------------- */

}

#endif
