#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <sstream>

#include "madeup/Environment.h"
#include "madeup/Expression.h"

/* class ExpressionBlock; */

using std::ostream;

namespace madeup {

class UnlocatedException : public td::MessagedException {
  public:
    UnlocatedException(const std::string& message) :
      MessagedException(message) {
    }
};

/* ------------------------------------------------------------------------- */

class Expression {
  public:
    Expression();
    virtual ~Expression();

    virtual Co<Expression> Evaluate(Environment& env) = 0;

    virtual void Write(ostream& out) const = 0;

    static Co<Expression> Parse(const std::string& s);

    int GetStartLine() const;
    int GetStartIndex() const;
    int GetEndLine() const;
    int GetEndIndex() const;
    const std::string& GetSource() const;
    std::string GetSourceLocation() const;

    void SetSource(const std::string& source, int start_line, int start_index, int end_line, int end_index);

  private:
    int start_line;
    int start_index;
    int end_line;
    int end_index;
    std::string source;
};

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out, const Co<Expression> e);

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out, const Expression& e);

/* ------------------------------------------------------------------------- */

}

#endif
