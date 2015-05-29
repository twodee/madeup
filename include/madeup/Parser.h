#ifndef PARSER_H
#define PARSER_H

#include <stack>
#include <vector>

#include "madeup/Token.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionBlock.h"
#include "twodee/Co.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class Parser {
  public:
    Parser(const std::vector<Token> &tokens);

    Co<ExpressionBlock> program();
    void block();
    void statement();
    void expressionLevel0();
    void expressionLevel1();
    void expressionLevel2();
    void expressionLevel3();
    void expressionLevel4();
    void expressionLevel5();
    void expressionLevel6();
    void atom();

  private:
    bool isUp(Token::token_t type, int k = 1) const;
    bool isInExpressionFirst(int k = 1) const;
    Co<Expression> popExpression();
    Co<ExpressionBlock> popBlock();

    const std::vector<Token> &tokens;
    std::stack<Co<Expression> > expressions;
    std::stack<Co<ExpressionBlock> > blocks;
    int i;
};

/* ------------------------------------------------------------------------- */

}

#endif
