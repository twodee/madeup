#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <sstream>

#include "madeup/Environment.h"
#include "madeup/Expression.h"
#include "madeup/SourceLocation.h"

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
    const std::string& GetSource() const;
    const SourceLocation &GetSourceLocation() const;
    void SetSource(const std::string& source, const SourceLocation &location);

    static Co<Expression> Parse(const std::string& s);

  private:
    std::string source;
    SourceLocation location;
};

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out, const Co<Expression> e);

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out, const Expression& e);

/* ------------------------------------------------------------------------- */

}

#endif
