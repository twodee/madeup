#include <iostream>

#include "madeup/Expression.h"
#include "madeup/ExpressionAbsoluteValue.h"
#include "madeup/ExpressionAdd.h"
#include "madeup/ExpressionAnd.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionBox.h"
#include "madeup/ExpressionCall.h"
#include "madeup/ExpressionCallWithNamedParameters.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionDefineArrayElement.h"
#include "madeup/ExpressionDefineVariable.h"
#include "madeup/ExpressionDivide.h"
#include "madeup/ExpressionEqual.h"
#include "madeup/ExpressionFor.h"
#include "madeup/ExpressionGreater.h"
#include "madeup/ExpressionGreaterOrEqual.h"
#include "madeup/ExpressionIf.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionLesser.h"
#include "madeup/ExpressionLesserOrEqual.h"
#include "madeup/ExpressionMultiply.h"
#include "madeup/ExpressionNegation.h"
#include "madeup/ExpressionNot.h"
#include "madeup/ExpressionNotEqual.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionOr.h"
#include "madeup/ExpressionPower.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionRealDivide.h"
#include "madeup/ExpressionRemainder.h"
#include "madeup/ExpressionRepeat.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionSubtract.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionWhile.h"
#include "madeup/Lexer.h"
#include "madeup/Parser.h"
#include "madeup/SourceLocation.h"
#include "madeup/Token.h"
#include "madeup/Parser.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

Parser::Parser(const std::vector<Token> &tokens) :
  tokens(tokens),
  expressions(),
  blocks(),
  i(0) {
}

/* ------------------------------------------------------------------------- */

bool Parser::isUp(Token::token_t type, int k) const {
  return i + k - 1 <= tokens.size() && tokens[i + k - 1].getType() == type;
}

/* ------------------------------------------------------------------------- */

bool Parser::isInExpressionFirst(int k) const {
  return isUp(Token::LEFT_PARENTHESIS, k) ||
         isUp(Token::PIPE, k) ||
         isUp(Token::INTEGER, k) ||
         isUp(Token::REAL, k) ||
         isUp(Token::STRING, k) ||
         isUp(Token::TRUE, k) ||
         isUp(Token::FALSE, k) ||
         isUp(Token::IF, k) ||
         isUp(Token::FOR, k) ||
         isUp(Token::REPEAT, k) ||
         isUp(Token::WHILE, k) ||
         isUp(Token::NOT, k) ||
         isUp(Token::TO, k) ||
         isUp(Token::ID, k);
}

/* ------------------------------------------------------------------------- */

Co<Expression> Parser::popExpression() {
  Co<Expression> e = expressions.top();
  expressions.pop();
  return e;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionBlock> Parser::popBlock() {
  Co<ExpressionBlock> block = blocks.top();
  blocks.pop();
  return block;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionBlock> Parser::program() {
  block();

  if (isUp(Token::END_OF_FILE)) {
    ++i;
  } else {
    std::stringstream ss;
    ss << tokens[i].getLocation().getStartRow() << "(" << tokens[i].getLocation().getStartColumn() << "-" << tokens[i].getLocation().getEndColumn() << "): I ran into \"" << tokens[i].getText() << "\" in a place I didn't expect it.";
    throw MessagedException(ss.str());
  }

  if (i != tokens.size()) {
    throw MessagedException("extra stuff at EOF");
  }

  return popBlock();
}

/* ------------------------------------------------------------------------- */

void Parser::block() {
  blocks.push(Co<ExpressionBlock>(new ExpressionBlock()));
  while (isUp(Token::NEWLINE) ||
         isInExpressionFirst()) {
    statement();
  } 
}

/* ------------------------------------------------------------------------- */

void Parser::statement() {
  if (isUp(Token::NEWLINE)) {
    ++i;
  } else {
    expressionLevel0();
    if (isUp(Token::NEWLINE)) {
      ++i;
      blocks.top()->AddExpression(popExpression());
    } else {
      throw MessagedException("expected end of line after statement");
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel0() {
  expressionLevel1();
  while (isUp(Token::OR)) {
    ++i;
    expressionLevel1();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    expressions.push(Co<Expression>(new ExpressionOr(a, b)));
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel1() {
  expressionLevel2();
  while (isUp(Token::AND)) {
    ++i;
    expressionLevel2();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    expressions.push(Co<Expression>(new ExpressionAnd(a, b)));
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel2() {
  expressionLevel3();
  while (isUp(Token::EQUAL_TO) || isUp(Token::NOT_EQUAL_TO)) {
    Token::token_t type = tokens[i].getType();
    ++i;
    expressionLevel3();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    if (type == Token::EQUAL_TO) {
      expressions.push(Co<Expression>(new ExpressionEqual(a, b)));
    } else {
      expressions.push(Co<Expression>(new ExpressionNotEqual(a, b)));
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel3() {
  expressionLevel4();
  while (isUp(Token::LESS_THAN) || isUp(Token::LESS_THAN_OR_EQUAL_TO) || isUp(Token::GREATER_THAN) || isUp(Token::GREATER_THAN_OR_EQUAL_TO)) {
    Token::token_t type = tokens[i].getType();
    ++i;
    expressionLevel4();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    if (type == Token::LESS_THAN) {
      expressions.push(Co<Expression>(new ExpressionLesser(a, b)));
    } else if (type == Token::LESS_THAN_OR_EQUAL_TO) {
      expressions.push(Co<Expression>(new ExpressionLesserOrEqual(a, b)));
    } else if (type == Token::GREATER_THAN) {
      expressions.push(Co<Expression>(new ExpressionGreater(a, b)));
    } else {
      expressions.push(Co<Expression>(new ExpressionGreaterOrEqual(a, b)));
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel4() {
  expressionLevel5();
  while (isUp(Token::PLUS) || isUp(Token::MINUS)) {
    Token::token_t type = tokens[i].getType();
    ++i;
    expressionLevel5();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    if (type == Token::PLUS) {
      expressions.push(Co<Expression>(new ExpressionAdd(a, b)));
    } else {
      expressions.push(Co<Expression>(new ExpressionSubtract(a, b)));
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel5() {
  expressionLevel6();
  while (isUp(Token::TIMES) || isUp(Token::DIVIDE) || isUp(Token::REAL_DIVIDE) || isUp(Token::REMAINDER)) {
    Token::token_t type = tokens[i].getType();
    ++i;
    expressionLevel6();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    if (type == Token::TIMES) {
      expressions.push(Co<Expression>(new ExpressionMultiply(a, b)));
    } else if (type == Token::DIVIDE) {
      expressions.push(Co<Expression>(new ExpressionDivide(a, b)));
    } else if (type == Token::REAL_DIVIDE) {
      expressions.push(Co<Expression>(new ExpressionRealDivide(a, b)));
    } else {
      expressions.push(Co<Expression>(new ExpressionRemainder(a, b)));
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel6() {
  expressionLevel7();
  while (isUp(Token::CIRCUMFLEX)) {
    ++i;
    expressionLevel7();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    expressions.push(Co<Expression>(new ExpressionPower(a, b)));
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel7() {
  expressionLevel8();
  if (isUp(Token::BY) || isUp(Token::OF)) {
    std::stack<Co<Expression> > dimensions;
    dimensions.push(popExpression());

    while (isUp(Token::BY)) {
      ++i;
      expressionLevel8();
      dimensions.push(popExpression());
    }

    if (isUp(Token::OF)) {
      ++i;
      expressionLevel8();
      Co<Expression> fill = popExpression();
      do {
        Co<Expression> dimension = dimensions.top();
        dimensions.pop();
        fill = Co<Expression>(new ExpressionArrayConstructor(fill, dimension));
      } while (dimensions.size() > 0);
      expressions.push(fill);
    } else {
      throw MessagedException("mssing of");
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel8() {
  if (isUp(Token::NOT)) {
    ++i;
    expressionLevel8();
    Co<Expression> e = popExpression();
    expressions.push(Co<Expression>(new ExpressionNot(e)));
  } else if (isUp(Token::MINUS)) {
    ++i;
    expressionLevel8();
    Co<Expression> e = popExpression();
    expressions.push(Co<Expression>(new ExpressionNegation(e)));
  } else {
    expressionLevel9();
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel9() {
  atom();
  while (isUp(Token::LEFT_BRACKET) || isUp(Token::DOT)) {
    if (isUp(Token::DOT)) {
      ++i;
      if (isUp(Token::ID) && tokens[i].getText() == "length") {
        std::string name = tokens[i].getText();
        ++i;
        expressions.push(Co<Expression>(new ExpressionArrayLength(popExpression())));
      } else {
        throw MessagedException("not legal after .");
      }
    } else { 
      ++i;
      expressionLevel0();
      if (isUp(Token::RIGHT_BRACKET)) {
        ++i;
        Co<Expression> b = popExpression();
        Co<Expression> a = popExpression();
        if (isUp(Token::ASSIGN)) {
          ++i;
          Co<Expression> rhs;
          if (isUp(Token::NEWLINE)) {
            ++i;
            block();
            rhs = popBlock();
            if (isUp(Token::END)) {
              ++i;
            } else {
              throw MessagedException("missing end");
            }
          } else {
            expressionLevel0();
            rhs = popExpression();
          }
          expressions.push(Co<Expression>(new ExpressionDefineArrayElement(a, b, rhs)));
        } else {
          expressions.push(Co<Expression>(new ExpressionArraySubscript(a, b)));
        }
      } else {
        throw MessagedException("missing ]");
      }
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::atom() {
  if (isUp(Token::LEFT_PARENTHESIS)) {
    ++i;
    expressionLevel0();
    if (isUp(Token::RIGHT_PARENTHESIS)) {
      ++i;
    } else {
      throw MessagedException("expecting )");
    }
  } else if (isUp(Token::PIPE)) {
    ++i;
    expressionLevel0();
    if (isUp(Token::PIPE)) {
      ++i;
      Co<Expression> e = popExpression();
      expressions.push(new ExpressionAbsoluteValue(e));
    } else {
      throw MessagedException("expecting |");
    }
  } else if (isUp(Token::INTEGER)) {
    int n = atoi(tokens[i].getText().c_str());
    expressions.push(Co<Expression>(new ExpressionInteger(n)));
    ++i;
  } else if (isUp(Token::REAL)) {
    float n = atof(tokens[i].getText().c_str());
    expressions.push(Co<Expression>(new ExpressionReal(n)));
    ++i;
  } else if (isUp(Token::TRUE)) {
    expressions.push(Co<Expression>(new ExpressionBoolean(true)));
    ++i;
  } else if (isUp(Token::FALSE)) {
    expressions.push(Co<Expression>(new ExpressionBoolean(false)));
    ++i;
  } else if (isUp(Token::STRING)) {
    expressions.push(Co<Expression>(new ExpressionString(tokens[i].getText())));
    ++i;
  } else if (isUp(Token::REPEAT)) {
    ++i;
    expressionLevel0();
    if (isUp(Token::NEWLINE)) {
      ++i;
      block();
      if (isUp(Token::END)) {
        ++i;
        Co<Expression> n = popExpression();
        Co<ExpressionBlock> block = popBlock();
        expressions.push(Co<Expression>(new ExpressionRepeat(n, block)));
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().getStartRow() << "(" << tokens[i].getLocation().getStartColumn() << "-" << tokens[i].getLocation().getEndColumn() << "): I ran into \"" << tokens[i].getText() << "\" in a place where I expected \"end\".";
        throw MessagedException(ss.str());
      }
    } else {
      throw MessagedException("expected newline");
    }
  } else if (isUp(Token::WHILE)) {
    ++i;
    expressionLevel0();
    if (isUp(Token::NEWLINE)) {
      ++i;
      block();
      if (isUp(Token::END)) {
        ++i;
        Co<Expression> condition = popExpression();
        Co<ExpressionBlock> block = popBlock();
        expressions.push(Co<Expression>(new ExpressionWhile(condition, block)));
      } else {
        throw MessagedException("expecting end after repeat");
      }
    } else {
      throw MessagedException("expected newline");
    }
  } else if (isUp(Token::IF)) {
    ++i;
    expressionLevel0();

    // One-liner.
    if (isUp(Token::THEN)) {
      ++i;
      expressionLevel0();
      Co<ExpressionBlock> then_block(new ExpressionBlock());
      then_block->AddExpression(popExpression());
      blocks.push(then_block);

      if (isUp(Token::ELSE)) {
        ++i;
        expressionLevel0();
        Co<ExpressionBlock> else_block(new ExpressionBlock());
        else_block->AddExpression(popExpression());
        blocks.push(else_block);
      } else {
        throw MessagedException("expecting else");
      }
    }

    // Block-based.
    else if (isUp(Token::NEWLINE)) {
      ++i;
      block();
      if (isUp(Token::ELSE)) {
        ++i;
        if (isUp(Token::NEWLINE)) {
          ++i;
          block();
          if (isUp(Token::END)) {
            ++i;
          } else {
            throw MessagedException("expecting end");
          }
        } else {
          throw MessagedException("expecting newline after else");
        }
      } else {
        blocks.push(Co<ExpressionBlock>(new ExpressionBlock()));
      }
    }

    else {
      throw MessagedException("expecting then or new line");
    }

    Co<ExpressionBlock> else_block = popBlock();
    Co<ExpressionBlock> then_block = popBlock();
    Co<Expression> condition = popExpression();
    expressions.push(Co<Expression>(new ExpressionIf(condition, then_block, else_block)));
  } else if (isUp(Token::TO)) {
    ++i;
    if (isUp(Token::ID)) {
      std::string name = tokens[i].getText();
      ++i;
      std::vector<std::string> formals;
      while (isUp(Token::ID)) {
        formals.push_back(tokens[i].getText());
        ++i;
      }
      Co<Expression> rhs;
      if (isUp(Token::ASSIGN)) {
        ++i;
        expressionLevel0();
        rhs = popExpression();
      } else if (isUp(Token::NEWLINE)) {
        ++i;
        block();
        if (isUp(Token::END)) {
          ++i;
          rhs = popBlock();
        } else {
          throw MessagedException("missing end");
        }
      } else {
        throw MessagedException("unexpected after func declare");
      }

      Co<ExpressionDefine> define(new ExpressionDefine(name, rhs));
      for (std::vector<std::string>::const_iterator formal = formals.begin(); formal != formals.end(); ++formal) {
        define->AddFormal(*formal);
      }
      expressions.push(define);
    } else {
      throw MessagedException("missing name");
    }
  } else if (isUp(Token::ID) && isUp(Token::ASSIGN, 2)) {
    std::string name = tokens[i].getText();
    i += 2;
    Co<Expression> rhs;
    if (isUp(Token::NEWLINE)) {
      ++i;
      block();
      if (isUp(Token::END)) {
        rhs = popBlock();
      } else {
        throw MessagedException("expecting end after assignment");
      }
    } else {
      expressionLevel0();
      rhs = popExpression();
    }

    Co<ExpressionDefineVariable> define;
    if (name == "seed") {
      define = Co<ExpressionDefineVariable>(new ExpressionDefineVariableSeed(name.c_str(), rhs));
    } else {
      define = Co<ExpressionDefineVariable>(new ExpressionDefineVariable(name.c_str(), rhs));
    }
    expressions.push(define);
  } else if (isUp(Token::ID)) {
    std::string name = tokens[i].getText();
    ++i;
    if (isUp(Token::ID) && isUp(Token::COLON, 2)) {
      /* std::cout << "name" << std::endl; */
      Co<ExpressionCallWithNamedParameters> call(new ExpressionCallWithNamedParameters(name));
      while (isUp(Token::ID) && isUp(Token::COLON, 2)) {
        std::string formal = tokens[i].getText();
        /* std::cout << "formal: " << formal << std::endl; */
        i += 2;
        expressionLevel0();
        Co<Expression> actual = popExpression();
        /* std::cout << "actual: " << actual << std::endl; */
        call->AddParameter(formal, actual);
      }
      expressions.push(call);
      /* std::cout << "call: " << call << std::endl; */
    } else {
      Co<ExpressionCall> call(new ExpressionCall(name));
      if (isInExpressionFirst()) {
        expressionLevel0();
        call->AddParameter(popExpression());
        while (isUp(Token::COMMA)) {
          ++i;
          if (isInExpressionFirst()) {
            expressionLevel0();
            call->AddParameter(popExpression());
          } else {
            throw MessagedException("dangling comma");
          }
        }
      }
      expressions.push(call);
    }
  } else if (isUp(Token::FOR)) {
    ++i;

    if (isUp(Token::ID)) {
      std::string iterator_id = tokens[i].getText();
      ++i;

      Co<Expression> start;
      Co<Expression> stop;
      Co<Expression> by;
      Co<ExpressionBlock> body;
      bool is_inclusive = true;

      if (isUp(Token::TO)) {
        ++i;
        is_inclusive = false;
        expressionLevel0();
        start = Co<Expression>(new ExpressionInteger(0));
        stop = popExpression();
        if (isUp(Token::BY)) {
          ++i;
          expressionLevel0();
          by = popExpression();
        } else {
          by = Co<Expression>(new ExpressionInteger(1));
        }
      } else if (isUp(Token::THROUGH)) {
        ++i;
        expressionLevel0();
        start = Co<Expression>(new ExpressionInteger(0));
        stop = popExpression();
        if (isUp(Token::BY)) {
          ++i;
          expressionLevel0();
          by = popExpression();
        } else {
          by = Co<Expression>(new ExpressionInteger(1));
        }
      } else if (isUp(Token::IN)) {
        ++i;
        expressionLevel0();
        start = popExpression();

        if (isUp(Token::COMMA)) {
          ++i;
          expressionLevel0();
          Co<Expression> second_term = popExpression();
          by = Co<Expression>(new ExpressionSubtract(second_term, start));
        }

        if (isUp(Token::RANGE)) {
          ++i;
          expressionLevel0();
          stop = popExpression();

          if (by.IsNull() && isUp(Token::BY)) {
            ++i;
            expressionLevel0();
            by = popExpression();
          } else if (by.IsNull()) {
            by = Co<Expression>(new ExpressionInteger(1));
          } else if (isUp(Token::BY)) {
            ++i;
            throw MessagedException("can't have by with second term");
          }
        } else {
          throw MessagedException("missing range");
        }
      } else {
        throw MessagedException("bad for loop");
      }

      if (isUp(Token::NEWLINE)) {
        ++i;
        block();
        body = popBlock();
        if (isUp(Token::END)) {
          ++i;
          expressions.push(Co<Expression>(new ExpressionFor(iterator_id, start, stop, by, body, is_inclusive)));
        } else {
          throw MessagedException("expecting end");
        }
      } else {
        throw MessagedException("expecting newline");
      }
    } else {
      throw MessagedException("missing id in for");
    }

  } else {
    std::stringstream ss;
    ss << tokens[i].getLocation().getStartRow() << "(" << tokens[i].getLocation().getStartColumn() << "-" << tokens[i].getLocation().getEndColumn() << "): I ran into \"" << tokens[i].getText() << "\" in a place I didn't expect it.";
    throw MessagedException(ss.str());
  }
}

/* ------------------------------------------------------------------------- */

}
