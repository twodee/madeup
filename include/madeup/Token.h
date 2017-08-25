#ifndef TOKEN_H
#define TOKEN_H

#include "madeup/SourceLocation.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class Token {
  public:
    enum token_t {
      LEFT_PARENTHESIS,
      RIGHT_PARENTHESIS,
      COLON,
      CIRCUMFLEX,
      ID,
      AT_SIGN,
      LEFT_BRACKET,
      RIGHT_BRACKET,
      LEFT_CURLY_BRACE,
      RIGHT_CURLY_BRACE,
      PLUS,
      MINUS,
      TIMES,
      DIVIDE,
      REAL_DIVIDE,
      INTEGER,
      REAL,
      STRING,
      COMMA,
      LESS_THAN,
      LESS_THAN_OR_EQUAL_TO,
      GREATER_THAN,
      GREATER_THAN_OR_EQUAL_TO,
      EQUAL_TO,
      NOT_EQUAL_TO,
      REMAINDER,
      ASSIGN,
      PIPE,
      RANGE,
      NEWLINE,
      AND,
      BY,
      ELSE,
      END,
      FALSE,
      FOR,
      IF,
      IN,
      NOT,
      OF,
      OR,
      REPEAT,
      THEN,
      THROUGH,
      TO,
      TRUE,
      WHILE,
      NOTHING,
      AROUND,
      COMMENT,
      END_OF_FILE,
      RETURN,
      UNKNOWN
    };

    Token(token_t type,
          const std::string &text,
          const SourceLocation &location);

    token_t getType() const;
    const std::string getText() const;
    const std::string getQuotedText() const;
    const SourceLocation &getLocation() const;
    const std::string getTypeString() const;
    bool isPrecededBySpace() const;
    void isPrecededBySpace(bool is);

  private:
    token_t type; 
    std::string text;
    SourceLocation location;
    bool is_preceded_by_space;
};

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const Token &token);

/* ------------------------------------------------------------------------- */

}

#endif
