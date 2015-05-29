#include "madeup/ExpressionAdd.h"
#include "madeup/ExpressionAnd.h"
#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionCall.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionDivide.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionEqual.h"
#include "madeup/ExpressionFor.h"
#include "madeup/ExpressionGreater.h"
#include "madeup/ExpressionGreaterOrEqual.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionIf.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionLesser.h"
#include "madeup/ExpressionLesserOrEqual.h"
#include "madeup/ExpressionMove.h"
#include "madeup/ExpressionMoveTo.h"
#include "madeup/ExpressionMultiply.h"
#include "madeup/ExpressionNotEqual.h"
#include "madeup/ExpressionNot.h"
#include "madeup/ExpressionOr.h"
#include "madeup/ExpressionRemainder.h"
#include "madeup/ExpressionRepeat.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionSubtract.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionWhile.h"

using std::ostream;

/* ------------------------------------------------------------------------- */

namespace madeup {

/* ------------------------------------------------------------------------- */

Expression::Expression() :
  start_line(0),
  start_index(0),
  end_line(0),
  end_index(0),
  source("n/a") {
}

/* ------------------------------------------------------------------------- */

Expression::~Expression() {
  /* std::cout << "deleting " << this << " [" << source << "]" << std::endl; */
}

/* ------------------------------------------------------------------------- */

int Expression::GetStartLine() const {
  return start_line;
}

/* ------------------------------------------------------------------------- */

int Expression::GetStartIndex() const {
  return start_index;
}

/* ------------------------------------------------------------------------- */

int Expression::GetEndLine() const {
  return end_line;
}

/* ------------------------------------------------------------------------- */

int Expression::GetEndIndex() const {
  return end_index;
}

/* ------------------------------------------------------------------------- */

const std::string& Expression::GetSource() const {
  return source;
}

/* ------------------------------------------------------------------------- */

void Expression::SetSource(const std::string& source, int start_line, int start_index, int end_line, int end_index) {
  this->source = source;
  this->start_line = start_line; 
  this->start_index = start_index;
  this->end_line = end_line;
  this->end_index = end_index;
}

/* ------------------------------------------------------------------------- */

std::string Expression::GetSourceLocation() const {
  std::stringstream ss;
  ss << GetStartLine() << "(" << GetStartIndex();
  if (GetStartIndex() != GetEndIndex()) {
    ss << "-" << GetEndIndex();
  }
  ss << ")";
  return ss.str();
}

/* ------------------------------------------------------------------------- */

ostream& operator<<(ostream& out, const Co<Expression> e) {
  e->Write(out);
  return out;
}

/* ------------------------------------------------------------------------- */

ostream& operator<<(ostream& out, const Expression& e) {
  e.Write(out);
  return out;
}

/* ------------------------------------------------------------------------- */

using std::stringstream;

Co<Expression> Parse(stringstream& ss) {
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
      block->AddExpression(Parse(ss));
      std::cerr << "block added" << std::endl;
    }
    std::cerr << "block done" << std::endl;
    expr = Co<Expression>(block);
  }
  
  else if (token.compare("if") == 0) {
    ss.get(); // eat space
    Co<Expression> condition = Parse(ss);
    std::cerr << "condition: " << condition << std::endl;
    ss.get(); // eat space
    Co<ExpressionBlock> then_block = Parse(ss);
    std::cerr << "then_block: " << then_block << std::endl;
    ss.get(); // eat space
    Co<ExpressionBlock> else_block = Parse(ss);
    std::cerr << "else_block: " << else_block << std::endl;
    expr = Co<Expression>(new ExpressionIf(condition, then_block, else_block));
  }
  
  else if (token.compare("while") == 0) {
    ss.get(); // eat space
    Co<Expression> condition = Parse(ss);
    std::cerr << "condition: " << condition << std::endl;
    ss.get(); // eat space
    Co<ExpressionBlock> block = Parse(ss);
    expr = Co<Expression>(new ExpressionWhile(condition, block));
  }

  else if (token.compare("for") == 0) {
    ss.get(); // eat space
    string iterator;
    ss >> iterator;
    ss.get(); // eat space
    Co<Expression> start = Parse(ss);
    ss.get(); // eat space
    Co<Expression> end = Parse(ss);
    ss.get(); // eat space
    Co<Expression> delta = Parse(ss);
    ss.get(); // eat space
    Co<ExpressionBlock> block = Parse(ss);
    expr = Co<Expression>(new ExpressionFor(iterator, start, end, delta, block, false));
  }
  
#define BINOP(op, clazz) \
  else if (token.compare(op) == 0) { \
    ss.get(); \
    Co<Expression> a = Parse(ss); \
    ss.get(); \
    Co<Expression> b = Parse(ss); \
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

    Co<Expression> body = Parse(ss);

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
      call->AddParameter(Parse(ss));
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

Co<Expression> Expression::Parse(const std::string& s) {
  stringstream ss(s);
  return madeup::Parse(ss);
}

/* ------------------------------------------------------------------------- */

}
