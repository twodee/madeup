#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <string>

#include "madeup/Environment.h"
#include "madeup/SourceLocation.h"

using std::ostream;

namespace madeup {

/* ------------------------------------------------------------------------- */

class UnlocatedException : public td::MessagedException {
  public:
    UnlocatedException(const std::string &message) :
      MessagedException(message) {
    }
};

/* ------------------------------------------------------------------------- */

class Expression {
  public:
    Expression();
    virtual ~Expression();

    virtual td::Co<Expression> evaluate(Environment &env) const = 0;
    virtual void write(ostream &out) const = 0;
    const std::string &getSource() const;
    const SourceLocation &getSourceLocation() const;
    void setSource(const std::string &source, const SourceLocation &location);
    virtual void predeclare(Environment &env) const;
    virtual void assign(Environment &env, td::Co<Expression> value) const;

    static td::Co<Expression> parse(const std::string &s);

  private:
    std::string source;
    SourceLocation location;
};

/* ------------------------------------------------------------------------- */

ostream &operator<<(ostream &out, const td::Co<Expression> e);

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const Expression &e);

/* ------------------------------------------------------------------------- */

}

#endif
