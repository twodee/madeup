#ifndef PARSER_H
#define PARSER_H

#include <stack>
#include <vector>

#include "madeup/Expression.h"
#include "madeup/ExpressionBlock.h"
#include "madeup/Token.h"
#include "twodee/Co.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class Parser {
  public:
    Parser(const std::vector<Token> &tokens,
           const std::string &source = "");

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
    void expressionLevel7();
    void expressionLevel8();
    void expressionLevel9();
    void atom();

  private:
    bool isUp(Token::token_t type, size_t k = 1) const;
    bool isInExpressionFirst(int k = 1) const;
    Co<Expression> popExpression();
    Co<ExpressionBlock> popBlock();
    std::string getSubsource(const SourceLocation &location) const;
    void pushExpression(Expression *e,
                        const SourceLocation &from,
                        const SourceLocation &to);

    const std::vector<Token> tokens;
    const std::string source;
    std::stack<Co<Expression> > expressions;
    std::stack<Co<ExpressionBlock> > blocks;
    size_t i;
    bool is_in_loop_range;
    bool is_in_pipe;
};

/* ------------------------------------------------------------------------- */

}

#endif
