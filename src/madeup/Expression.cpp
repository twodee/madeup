#include "madeup/Expression.h"
#include "madeup/ExpressionAdd.h"
#include "madeup/ExpressionAnd.h"
#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionCall.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionDivide.h"
#include "madeup/ExpressionEqual.h"
#include "madeup/ExpressionFor.h"
#include "madeup/ExpressionGreater.h"
#include "madeup/ExpressionGreaterOrEqual.h"
#include "madeup/ExpressionIf.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionLesser.h"
#include "madeup/ExpressionLesserOrEqual.h"
#include "madeup/ExpressionMove.h"
#include "madeup/ExpressionMoveTo.h"
#include "madeup/ExpressionMultiply.h"
#include "madeup/ExpressionNot.h"
#include "madeup/ExpressionNotEqual.h"
#include "madeup/ExpressionOr.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionRemainder.h"
#include "madeup/ExpressionRepeat.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionSubtract.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionWhile.h"

using std::ostream;
using namespace td;

/* ------------------------------------------------------------------------- */

namespace madeup {

/* ------------------------------------------------------------------------- */

Expression::Expression() :
  source("n/a"),
  location() {
}

/* ------------------------------------------------------------------------- */

Expression::~Expression() {
  /* std::cout << "deleting " << this << " [" << source << "]" << std::endl; */
}

/* ------------------------------------------------------------------------- */

const std::string &Expression::getSource() const {
  return source;
}

/* ------------------------------------------------------------------------- */

void Expression::setSource(const std::string &source,
                           const SourceLocation &location) {
  this->source = source;
  this->location = location;
}

/* ------------------------------------------------------------------------- */

const SourceLocation &Expression::getSourceLocation() const {
  return location;
}

/* ------------------------------------------------------------------------- */

void Expression::predeclare(Environment &env) const {
  // Nop. Only function definitions get predeclared.
}

/* ------------------------------------------------------------------------- */

ostream &operator<<(ostream &out, const Co<Expression> e) {
  e->write(out);
  return out;
}

/* ------------------------------------------------------------------------- */

ostream &operator<<(ostream &out, const Expression &e) {
  e.write(out);
  return out;
}

/* ------------------------------------------------------------------------- */

using std::stringstream;

Co<Expression> parse(stringstream &ss) {
  // First consume left parenthesis.
  int c = ss.get();

  if (c != '(') {
    string line;
    getline(ss, line);
    std::cerr << (char) c << line << std::endl;
    assert(c == '(');
  }

  // Next grab expression token.
  Co<Expression> expr;
  string token;
  while (ss.peek() != ')' && ss.peek() != ' ') {
    token += ss.get();
  }
  /* string token; */
  /* ss >> token; */
  std::cerr << "token: " << token << std::endl;

  if (token.compare("block") == 0) {
    ExpressionBlock *block = new ExpressionBlock();
    std::cerr << "block started" << std::endl;
    while (ss.peek() == ' ') {
      ss.get(); // eat space
      block->append(parse(ss));
      std::cerr << "block added" << std::endl;
    }
    std::cerr << "block done" << std::endl;
    expr = Co<Expression>(block);
  }
  
  else if (token.compare("if") == 0) {
    ss.get(); // eat space
    Co<Expression> condition = parse(ss);
    std::cerr << "condition: " << condition << std::endl;
    ss.get(); // eat space
    Co<ExpressionBlock> then_block = parse(ss);
    std::cerr << "then_block: " << then_block << std::endl;
    ss.get(); // eat space
    Co<ExpressionBlock> else_block = parse(ss);
    std::cerr << "else_block: " << else_block << std::endl;
    expr = Co<Expression>(new ExpressionIf(condition, then_block, else_block));
  }
  
  else if (token.compare("while") == 0) {
    ss.get(); // eat space
    Co<Expression> condition = parse(ss);
    std::cerr << "condition: " << condition << std::endl;
    ss.get(); // eat space
    Co<ExpressionBlock> block = parse(ss);
    expr = Co<Expression>(new ExpressionWhile(condition, block));
  }

  else if (token.compare("for") == 0) {
    ss.get(); // eat space
    string iterator;
    ss >> iterator;
    ss.get(); // eat space
    Co<Expression> start = parse(ss);
    ss.get(); // eat space
    Co<Expression> end = parse(ss);
    ss.get(); // eat space
    Co<Expression> delta = parse(ss);
    ss.get(); // eat space
    Co<ExpressionBlock> block = parse(ss);
    expr = Co<Expression>(new ExpressionFor(Co<Expression>(new ExpressionCall(iterator)), start, end, delta, block, false));
  }
  
#define BINOP(op, clazz) \
  else if (token.compare(op) == 0) { \
    ss.get(); \
    Co<Expression> a = parse(ss); \
    ss.get(); \
    Co<Expression> b = parse(ss); \
    expr = Co<Expression>(new clazz(a, b)); \
  }

  BINOP("==", ExpressionEqual)
  BINOP("!=", ExpressionNotEqual)
  BINOP(">=", ExpressionGreaterOrEqual)
  BINOP(">", ExpressionGreater)
  BINOP("<=", ExpressionLesserOrEqual)
  BINOP("<", ExpressionLesser)
  BINOP("*", ExpressionMultiply)
  BINOP("-", ExpressionSubtract)
  BINOP("/", ExpressionDivide)
  BINOP("+", ExpressionAdd)
  
  else if (token.compare("INTEGER") == 0) {
    ss.get(); // eat space
    int i;
    ss >> i;
    expr = Co<Expression>(new ExpressionInteger(i));
  }

  else if (token.compare("STRING") == 0) {
    ss.get(); // eat space
    string s;
    while (ss.peek() != ')') {
      s += ss.get();
    }
    expr = Co<Expression>(new ExpressionString(s));
  }

  else if (token.compare("DECIMAL") == 0) {
    ss.get(); // eat space
    float f;
    ss >> f;
    expr = Co<Expression>(new ExpressionReal(f));
  }

  else if (token.compare("define") == 0) {
    ss.get(); // eat space
    string name;
    ss >> name;

    ss.get(); // eat space

    vector<FormalParameter> formals;
    while (ss.peek() != '(') {
      string formal_name;
      ss >> formal_name;
      formals.push_back(FormalParameter(formal_name, FormalParameter::EAGER));
      ss.get(); // eat space
    }

    Co<Expression> body = parse(ss);

    expr = Co<Expression>(new ExpressionDefine(name, body, formals));
  }

  else if (token.compare("call") == 0) {
    ss.get(); // eat space
    string name;

    while (ss.peek() != ')' && ss.peek() != ' ') {
      name += ss.get();
    }

    ExpressionCall *call = new ExpressionCall(name);

    while (ss.peek() == ' ') {
      ss.get(); // eat space
      call->addParameter(parse(ss));
    }

    expr = Co<Expression>(call);
  }

  c = ss.get();

  if (c != ')') {
    string line;
    getline(ss, line);
    std::cerr << (char) c << line << std::endl;
    std::cerr << "mismatched on token: " << token << std::endl;
    std::cerr << "expr: " << expr << std::endl;
    assert(c == ')');
  }

  std::cerr << "*expr: " << *expr << std::endl;

  return expr;
}

Co<Expression> Expression::parse(const std::string &s) {
  stringstream ss(s);
  return madeup::parse(ss);
}

/* ------------------------------------------------------------------------- */

void Expression::assign(Environment &env, Co<Expression> value) const {
  throw MessagedException("Assignment not supported."); 
}

/* ------------------------------------------------------------------------- */

}
