#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "madeup/Token.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class Lexer {
  public:
    Lexer(std::istream &in);
    const std::vector<Token> &getTokens() const;
    const std::vector<Token> &lex();

    void keepComments(bool enabled);

  private:
    Token makeToken(Token::token_t type);
    Token getToken();
    Token getTokenAfterSpace();
    Token getTokenAfterSlash();
    Token getTokenAfterDot();
    Token getTokenAfterMinus();
    Token getTokenAfterMinusMinus();
    Token getTokenAfterQuote();
    Token getTokenAfterLetter();
    Token getTokenAfterDigit();
    Token getTokenAfterLessThan();
    Token getTokenAfterGreaterThan();
    Token getTokenAfterEquals();
    Token getTokenAfterBang();

    std::vector<Token> tokens;
    std::istream &in;
    SourceLocation location;
    std::string text_so_far;
    bool keep_comments;
};

/* ------------------------------------------------------------------------- */

}

#endif
