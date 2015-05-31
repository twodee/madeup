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

Parser::Parser(const std::vector<Token> &tokens,
               const std::string &source) :
  tokens(tokens),
  source(source),
  expressions(),
  blocks(),
  i(0),
  is_in_loop_range(false),
  is_in_pipe(false) {
}

/* ------------------------------------------------------------------------- */

bool Parser::isUp(Token::token_t type, int k) const {
  return i + k - 1 <= tokens.size() && tokens[i + k - 1].getType() == type;
}

/* ------------------------------------------------------------------------- */

bool Parser::isInExpressionFirst(int k) const {
  return isUp(Token::LEFT_PARENTHESIS, k) ||
         (!is_in_pipe && isUp(Token::PIPE, k)) ||
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

void Parser::pushExpression(Expression *e,
                            const SourceLocation &from,
                            const SourceLocation &to) {
  Co<Expression> ee(e);
  SourceLocation location(from, to);
  ee->SetSource(getSubsource(location), location);
  expressions.push(ee);
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

std::string Parser::getSubsource(const SourceLocation &location) const {
  return source.substr(location.getStartIndex(), location.getEndIndex() - location.getStartIndex() + 1);
}

/* ------------------------------------------------------------------------- */

Co<ExpressionBlock> Parser::program() {
  block();

  if (isUp(Token::END_OF_FILE)) {
    ++i;
  } else {
    std::stringstream ss;
    ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place I didn't expect it.";
    throw MessagedException(ss.str());
  }

  if (i != tokens.size()) {
    std::stringstream ss;
    ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place I didn't expect it.";
    throw MessagedException(ss.str());
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
  Co<ExpressionBlock> block = blocks.top();
  SourceLocation location((*block)[0]->GetSourceLocation(), (*block)[block->GetLength() - 1]->GetSourceLocation());
  block->SetSource(getSubsource(location), location);
}

/* ------------------------------------------------------------------------- */

void Parser::statement() {
  if (isUp(Token::NEWLINE)) {
    ++i;
  } else {
    expressionLevel0();
    if (isUp(Token::NEWLINE)) {
      ++i;
      blocks.top()->Append(popExpression());
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a linebreak.";
      throw MessagedException(ss.str());
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
    pushExpression(new ExpressionOr(a, b), a->GetSourceLocation(), b->GetSourceLocation());
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
    pushExpression(new ExpressionAnd(a, b), a->GetSourceLocation(), b->GetSourceLocation());
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
      pushExpression(new ExpressionEqual(a, b), a->GetSourceLocation(), b->GetSourceLocation());
    } else {
      pushExpression(new ExpressionNotEqual(a, b), a->GetSourceLocation(), b->GetSourceLocation());
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
      pushExpression(new ExpressionLesser(a, b), a->GetSourceLocation(), b->GetSourceLocation());
    } else if (type == Token::LESS_THAN_OR_EQUAL_TO) {
      pushExpression(new ExpressionLesserOrEqual(a, b), a->GetSourceLocation(), b->GetSourceLocation());
    } else if (type == Token::GREATER_THAN) {
      pushExpression(new ExpressionGreater(a, b), a->GetSourceLocation(), b->GetSourceLocation());
    } else {
      pushExpression(new ExpressionGreaterOrEqual(a, b), a->GetSourceLocation(), b->GetSourceLocation());
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
      pushExpression(new ExpressionAdd(a, b), a->GetSourceLocation(), b->GetSourceLocation());
    } else {
      pushExpression(new ExpressionSubtract(a, b), a->GetSourceLocation(), b->GetSourceLocation());
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
      pushExpression(new ExpressionMultiply(a, b), a->GetSourceLocation(), b->GetSourceLocation());
    } else if (type == Token::DIVIDE) {
      pushExpression(new ExpressionDivide(a, b), a->GetSourceLocation(), b->GetSourceLocation());
    } else if (type == Token::REAL_DIVIDE) {
      pushExpression(new ExpressionRealDivide(a, b), a->GetSourceLocation(), b->GetSourceLocation());
    } else {
      pushExpression(new ExpressionRemainder(a, b), a->GetSourceLocation(), b->GetSourceLocation());
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
    pushExpression(new ExpressionPower(a, b), a->GetSourceLocation(), b->GetSourceLocation());
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel7() {
  expressionLevel8();
  if (!is_in_loop_range && (isUp(Token::BY) || isUp(Token::OF))) {
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
        Co<Expression> tmp = Co<Expression>(new ExpressionArrayConstructor(fill, dimension));
        SourceLocation location(dimension->GetSourceLocation(), fill->GetSourceLocation());
        tmp->SetSource(getSubsource(location), location);
        fill = tmp;
      } while (dimensions.size() > 0);
      expressions.push(fill);
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'of'.";
      throw MessagedException(ss.str());
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel8() {
  if (isUp(Token::NOT)) {
    ++i;
    expressionLevel8();
    Co<Expression> e = popExpression();
    pushExpression(new ExpressionNot(e), e->GetSourceLocation(), e->GetSourceLocation());
  } else if (isUp(Token::MINUS)) {
    ++i;
    expressionLevel8();
    Co<Expression> e = popExpression();
    pushExpression(new ExpressionNegation(e), e->GetSourceLocation(), e->GetSourceLocation());
  } else {
    expressionLevel9();
  }
}

/* ------------------------------------------------------------------------- */

void Parser::expressionLevel9() {
  atom();
  Co<Expression> array;
  if (isUp(Token::LEFT_BRACKET) || isUp(Token::DOT)) {
    array = popExpression();
  }
  while (isUp(Token::LEFT_BRACKET) || isUp(Token::DOT)) {
    if (isUp(Token::DOT)) {
      ++i;
      if (isUp(Token::ID) && tokens[i].getText() == "length") {
        pushExpression(new ExpressionArrayLength(array), array->GetSourceLocation(), tokens[i].getLocation());
        ++i;
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'length'.";
        throw MessagedException(ss.str());
      }
    } else { 
      ++i;
      expressionLevel0();
      Co<Expression> subscript = popExpression();
      if (isUp(Token::RIGHT_BRACKET)) {
        ++i;
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
              std::stringstream ss;
              ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
              throw MessagedException(ss.str());
            }
          } else {
            expressionLevel0();
            rhs = popExpression();
          }
          pushExpression(new ExpressionDefineArrayElement(array, subscript, rhs), array->GetSourceLocation(), rhs->GetSourceLocation());
        } else {
          pushExpression(new ExpressionArraySubscript(array, subscript), array->GetSourceLocation(), subscript->GetSourceLocation());
        }
      } else {
        std::stringstream ss;
        ss << subscript->GetSourceLocation().toAnchor() << ": I didn't find a ']' where I expected it.";
        throw MessagedException(ss.str());
      }
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::atom() {
  if (isUp(Token::LEFT_PARENTHESIS)) {
    ++i;
    expressionLevel0();
    Co<Expression> e = popExpression();
    if (isUp(Token::RIGHT_PARENTHESIS)) {
      ++i;
      expressions.push(e);
    } else {
      std::stringstream ss;
      ss << e->GetSourceLocation().toAnchor() << ": I didn't find a ')' where I expected it.";
      throw MessagedException(ss.str());
    }
  } else if (!is_in_pipe && isUp(Token::PIPE)) {
    Token left_pipe_token = tokens[i];
    ++i;
    is_in_pipe = true;
    expressionLevel0();
    is_in_pipe = false;
    Co<Expression> e = popExpression();
    if (isUp(Token::PIPE)) {
      pushExpression(new ExpressionAbsoluteValue(e), left_pipe_token.getLocation(), tokens[i].getLocation());
      ++i;
    } else {
      std::stringstream ss;
      ss << e->GetSourceLocation().toAnchor() << ": I didn't find a closing '|' where I expected it.";
      throw MessagedException(ss.str());
    }
  } else if (isUp(Token::INTEGER)) {
    int n = atoi(tokens[i].getText().c_str());
    expressions.push(Co<Expression>(new ExpressionInteger(n)));
    expressions.top()->SetSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::REAL)) {
    float n = atof(tokens[i].getText().c_str());
    expressions.push(Co<Expression>(new ExpressionReal(n)));
    expressions.top()->SetSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::TRUE)) {
    expressions.push(Co<Expression>(new ExpressionBoolean(true)));
    expressions.top()->SetSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::FALSE)) {
    expressions.push(Co<Expression>(new ExpressionBoolean(false)));
    expressions.top()->SetSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::STRING)) {
    expressions.push(Co<Expression>(new ExpressionString(tokens[i].getText())));
    expressions.top()->SetSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::REPEAT)) {
    Token repeat_token = tokens[i];
    ++i;
    expressionLevel0();
    if (isUp(Token::NEWLINE)) {
      ++i;
      block();
      if (isUp(Token::END)) {
        Co<Expression> n = popExpression();
        Co<ExpressionBlock> block = popBlock();
        pushExpression(new ExpressionRepeat(n, block), repeat_token.getLocation(), tokens[i].getLocation());
        ++i;
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
        throw MessagedException(ss.str());
      }
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a linebreak.";
      throw MessagedException(ss.str());
    }
  } else if (isUp(Token::WHILE)) {
    Token while_token = tokens[i];
    ++i;
    expressionLevel0();
    if (isUp(Token::NEWLINE)) {
      ++i;
      block();
      if (isUp(Token::END)) {
        Co<Expression> condition = popExpression();
        Co<ExpressionBlock> block = popBlock();
        pushExpression(new ExpressionWhile(condition, block), while_token.getLocation(), tokens[i].getLocation());
        ++i;
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
        throw MessagedException(ss.str());
      }
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a linebreak.";
      throw MessagedException(ss.str());
    }
  } else if (isUp(Token::IF)) {
    Token if_token = tokens[i];
    SourceLocation end_location;
    ++i;
    expressionLevel0();

    // One-liner.
    if (isUp(Token::THEN)) {
      ++i;
      expressionLevel0();
      Co<ExpressionBlock> then_block(new ExpressionBlock());
      Co<Expression> then_expression = popExpression();
      then_block->Append(then_expression);
      then_block->SetSource(then_expression->GetSource(), then_expression->GetSourceLocation());
      blocks.push(then_block);

      if (isUp(Token::ELSE)) {
        ++i;
        expressionLevel0();
        Co<ExpressionBlock> else_block(new ExpressionBlock());
        Co<Expression> else_expression = popExpression();
        else_block->Append(else_expression);
        else_block->Append(else_expression);
        else_block->SetSource(else_expression->GetSource(), else_expression->GetSourceLocation());
        blocks.push(else_block);
        end_location = else_block->GetSourceLocation();
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'else'.";
        throw MessagedException(ss.str());
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
            end_location = tokens[i].getLocation();
            ++i;
          } else {
            std::stringstream ss;
            ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
            throw MessagedException(ss.str());
          }
        } else {
          std::stringstream ss;
          ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a linebreak.";
          throw MessagedException(ss.str());
        }
      } else {
        blocks.push(Co<ExpressionBlock>(new ExpressionBlock()));
      }
    }

    else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'then' or a linebreak.";
      throw MessagedException(ss.str());
    }

    Co<ExpressionBlock> else_block = popBlock();
    Co<ExpressionBlock> then_block = popBlock();
    Co<Expression> condition = popExpression();
    pushExpression(new ExpressionIf(condition, then_block, else_block), if_token.getLocation(), end_location);
  } else if (isUp(Token::TO)) {
    Token to_token = tokens[i];
    SourceLocation end_location;
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
        end_location = rhs->GetSourceLocation();
      } else if (isUp(Token::NEWLINE)) {
        ++i;
        block();
        if (isUp(Token::END)) {
          end_location = tokens[i].getLocation();
          ++i;
          rhs = popBlock();
        } else {
          std::stringstream ss;
          ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
          throw MessagedException(ss.str());
        }
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected an assignment.";
        throw MessagedException(ss.str());
      }

      Co<ExpressionDefine> define(new ExpressionDefine(name, rhs));
      for (std::vector<std::string>::const_iterator formal = formals.begin(); formal != formals.end(); ++formal) {
        define->AddFormal(*formal);
      }
      SourceLocation location(to_token.getLocation(), end_location);
      define->SetSource(getSubsource(location), location);
      expressions.push(define);
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a name.";
      throw MessagedException(ss.str());
    }
  } else if (isUp(Token::ID) && isUp(Token::ASSIGN, 2)) {
    Token id_token = tokens[i];
    SourceLocation end_location;
    std::string name = tokens[i].getText();
    i += 2;
    Co<Expression> rhs;
    if (isUp(Token::NEWLINE)) {
      ++i;
      block();
      if (isUp(Token::END)) {
        end_location = tokens[i].getLocation();
        rhs = popBlock();
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
        throw MessagedException(ss.str());
      }
    } else {
      expressionLevel0();
      rhs = popExpression();
      end_location = rhs->GetSourceLocation();
    }

    Co<ExpressionDefineVariable> define;
    if (name == "seed") {
      pushExpression(new ExpressionDefineVariableSeed(name.c_str(), rhs), id_token.getLocation(), end_location);
    } else {
      pushExpression(new ExpressionDefineVariable(name.c_str(), rhs), id_token.getLocation(), end_location);
    }
  } else if (isUp(Token::ID)) {
    Token id_token = tokens[i];
    SourceLocation end_location;
    std::string name = tokens[i].getText();
    ++i;
    if (isUp(Token::ID) && isUp(Token::COLON, 2)) {
      Co<ExpressionCallWithNamedParameters> call(new ExpressionCallWithNamedParameters(name));
      while (isUp(Token::ID) && isUp(Token::COLON, 2)) {
        std::string formal = tokens[i].getText();
        i += 2;
        expressionLevel0();
        Co<Expression> actual = popExpression();
        end_location = actual->GetSourceLocation();
        call->AddParameter(formal, actual);
      }
      SourceLocation location(id_token.getLocation(), end_location);
      call->SetSource(getSubsource(location), location);
      expressions.push(call);
    } else {
      Co<ExpressionCall> call(new ExpressionCall(name));
      if (isInExpressionFirst()) {
        expressionLevel0();
        Co<Expression> actual = popExpression();
        call->AddParameter(actual);
        end_location = actual->GetSourceLocation();
        while (isUp(Token::COMMA)) {
          ++i;
          if (isInExpressionFirst()) {
            expressionLevel0();
            actual = popExpression();
            call->AddParameter(actual);
            end_location = actual->GetSourceLocation();
          } else {
            std::stringstream ss;
            ss << tokens[i - 1].getLocation().toAnchor() << ": I found an extra comma lying around.";
            throw MessagedException(ss.str());
          }
        }
      }
      SourceLocation location(id_token.getLocation(), end_location);
      call->SetSource(getSubsource(location), location);
      expressions.push(call);
    }
  } else if (isUp(Token::FOR)) {
    Token for_token = tokens[i];
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
          is_in_loop_range = true;
          expressionLevel0();
          is_in_loop_range = false;
          stop = popExpression();

          if (by.IsNull() && isUp(Token::BY)) {
            ++i;
            expressionLevel0();
            by = popExpression();
          } else if (by.IsNull()) {
            by = Co<Expression>(new ExpressionInteger(1));
          } else if (isUp(Token::BY)) {
            ++i;
            std::stringstream ss;
            ss << tokens[i - 1].getLocation().toAnchor() << ": I found a by clause in a for loop that already had its step determined.";
            throw MessagedException(ss.str());
          }
        } else {
          std::stringstream ss;
          ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected '..'.";
          throw MessagedException(ss.str());
        }
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'in', 'through', or 'to'.";
        throw MessagedException(ss.str());
      }

      if (isUp(Token::NEWLINE)) {
        ++i;
        block();
        body = popBlock();
        if (isUp(Token::END)) {
          pushExpression(new ExpressionFor(iterator_id, start, stop, by, body, is_inclusive), for_token.getLocation(), tokens[i].getLocation());
          ++i;
        } else {
          std::stringstream ss;
          ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
          throw MessagedException(ss.str());
        }
      } else {
        std::stringstream ss;
        ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a linebreak.";
        throw MessagedException(ss.str());
      }
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a name.";
      throw MessagedException(ss.str());
    }

  } else {
    std::stringstream ss;
    ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place I didn't expect it.";
    throw MessagedException(ss.str());
  }
}

/* ------------------------------------------------------------------------- */

}
