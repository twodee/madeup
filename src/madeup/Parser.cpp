#include <iostream>

#include "madeup/Expression.h"
#include "madeup/ExpressionAbsoluteValue.h"
#include "madeup/ExpressionAdd.h"
#include "madeup/ExpressionAnd.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionCall.h"
#include "madeup/ExpressionCallWithNamedParameters.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionDefineElement.h"
#include "madeup/ExpressionDefineVariable.h"
#include "madeup/ExpressionDivide.h"
#include "madeup/ExpressionDynamicMinus.h"
#include "madeup/ExpressionEqual.h"
#include "madeup/ExpressionFor.h"
#include "madeup/ExpressionGreater.h"
#include "madeup/ExpressionGreaterOrEqual.h"
#include "madeup/ExpressionIdentifier.h"
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
#include "madeup/ExpressionRepeatwich.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionSubscript.h"
#include "madeup/ExpressionSubrange.h"
#include "madeup/ExpressionSubtract.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionUnknown.h"
#include "madeup/ExpressionWhile.h"
#include "madeup/Lexer.h"
#include "madeup/Parser.h"
#include "madeup/SourceLocation.h"
#include "madeup/Token.h"
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

bool Parser::isUp(Token::token_t type, size_t k) const {
  return i + k - 1 <= tokens.size() && tokens[i + k - 1].getType() == type;
}

/* ------------------------------------------------------------------------- */

bool Parser::isInExpressionFirst(int k) const {
  return isUp(Token::LEFT_PARENTHESIS, k) ||
         (!is_in_pipe && isUp(Token::PIPE, k)) ||
         (!is_in_loop_range && isUp(Token::TO, k)) ||
         isUp(Token::INTEGER, k) ||
         isUp(Token::REAL, k) ||
         isUp(Token::STRING, k) ||
         isUp(Token::TRUE, k) ||
         isUp(Token::FALSE, k) ||
         isUp(Token::AT_SIGN, k) ||
         isUp(Token::UNKNOWN, k) ||
         isUp(Token::IF, k) ||
         isUp(Token::FOR, k) ||
         isUp(Token::REPEAT, k) ||
         isUp(Token::WHILE, k) ||
         isUp(Token::NOT, k) ||
         isUp(Token::NOTHING, k) ||
         isUp(Token::MINUS, k) ||
         isUp(Token::LEFT_CURLY_BRACE, k) ||
         isUp(Token::ID, k);
}

/* ------------------------------------------------------------------------- */

void Parser::pushExpression(Expression *e,
                            const SourceLocation &from,
                            const SourceLocation &to) {
  Co<Expression> ee(e);
  SourceLocation location(from, to);
  ee->setSource(getSubsource(location), location);
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

  // Only if the block is non-empty do we have source to register.
  Co<ExpressionBlock> block = blocks.top();
  if (block->getLength() > 0) {
    SourceLocation location((*block)[0]->getSourceLocation(), (*block)[block->getLength() - 1]->getSourceLocation());
    block->setSource(getSubsource(location), location);
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
      blocks.top()->append(popExpression());
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a linebreak.";
      throw MessagedException(ss.str());
    }
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 0: or.
void Parser::expressionLevel0() {
  expressionLevel1();
  while (isUp(Token::OR)) {
    ++i;
    expressionLevel1();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    pushExpression(new ExpressionOr(a, b), a->getSourceLocation(), b->getSourceLocation());
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 1: and.
void Parser::expressionLevel1() {
  expressionLevel2();
  while (isUp(Token::AND)) {
    ++i;
    expressionLevel2();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    pushExpression(new ExpressionAnd(a, b), a->getSourceLocation(), b->getSourceLocation());
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 2: not.
void Parser::expressionLevel2() {
  if (isUp(Token::NOT)) {
    ++i;
    expressionLevel2();
    Co<Expression> e = popExpression();
    pushExpression(new ExpressionNot(e), e->getSourceLocation(), e->getSourceLocation());
  } else {
    expressionLevel3();
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 3: == !=.
void Parser::expressionLevel3() {
  expressionLevel4();
  while (isUp(Token::EQUAL_TO) || isUp(Token::NOT_EQUAL_TO)) {
    Token::token_t type = tokens[i].getType();
    ++i;
    expressionLevel4();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    if (type == Token::EQUAL_TO) {
      pushExpression(new ExpressionEqual(a, b), a->getSourceLocation(), b->getSourceLocation());
    } else {
      pushExpression(new ExpressionNotEqual(a, b), a->getSourceLocation(), b->getSourceLocation());
    }
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 4: < <= > >=.
void Parser::expressionLevel4() {
  expressionLevel5();
  while (isUp(Token::LESS_THAN) || isUp(Token::LESS_THAN_OR_EQUAL_TO) || isUp(Token::GREATER_THAN) || isUp(Token::GREATER_THAN_OR_EQUAL_TO)) {
    Token::token_t type = tokens[i].getType();
    ++i;
    expressionLevel5();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    if (type == Token::LESS_THAN) {
      pushExpression(new ExpressionLesser(a, b), a->getSourceLocation(), b->getSourceLocation());
    } else if (type == Token::LESS_THAN_OR_EQUAL_TO) {
      pushExpression(new ExpressionLesserOrEqual(a, b), a->getSourceLocation(), b->getSourceLocation());
    } else if (type == Token::GREATER_THAN) {
      pushExpression(new ExpressionGreater(a, b), a->getSourceLocation(), b->getSourceLocation());
    } else {
      pushExpression(new ExpressionGreaterOrEqual(a, b), a->getSourceLocation(), b->getSourceLocation());
    }
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 5: + -.
void Parser::expressionLevel5() {
  expressionLevel6();
  while (isUp(Token::PLUS) || isUp(Token::MINUS)) {
    Token::token_t type = tokens[i].getType();
    ++i;
    expressionLevel6();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    if (type == Token::PLUS) {
      pushExpression(new ExpressionAdd(a, b), a->getSourceLocation(), b->getSourceLocation());
    } else {
      pushExpression(new ExpressionSubtract(a, b), a->getSourceLocation(), b->getSourceLocation());
    }
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 6: * / // %.
void Parser::expressionLevel6() {
  expressionLevel7();
  while (isUp(Token::TIMES) || isUp(Token::DIVIDE) || isUp(Token::REAL_DIVIDE) || isUp(Token::REMAINDER)) {
    Token::token_t type = tokens[i].getType();
    ++i;
    expressionLevel7();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    if (type == Token::TIMES) {
      pushExpression(new ExpressionMultiply(a, b), a->getSourceLocation(), b->getSourceLocation());
    } else if (type == Token::DIVIDE) {
      pushExpression(new ExpressionDivide(a, b), a->getSourceLocation(), b->getSourceLocation());
    } else if (type == Token::REAL_DIVIDE) {
      pushExpression(new ExpressionRealDivide(a, b), a->getSourceLocation(), b->getSourceLocation());
    } else {
      pushExpression(new ExpressionRemainder(a, b), a->getSourceLocation(), b->getSourceLocation());
    }
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 7: ^.
void Parser::expressionLevel7() {
  expressionLevel8();
  while (isUp(Token::CIRCUMFLEX)) {
    ++i;
    expressionLevel8();
    Co<Expression> b = popExpression();
    Co<Expression> a = popExpression();
    pushExpression(new ExpressionPower(a, b), a->getSourceLocation(), b->getSourceLocation());
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 8: expr BY expr OF.
void Parser::expressionLevel8() {
  expressionLevel9();
  if (!is_in_loop_range && (isUp(Token::BY) || isUp(Token::OF))) {
    std::stack<Co<Expression> > dimensions;
    dimensions.push(popExpression());

    // Read the higher-dimensional BY expressions...
    while (isUp(Token::BY)) {
      ++i;
      expressionLevel9();
      dimensions.push(popExpression());
    }

    // Read the filler value.
    Co<Expression> fill;
    if (isUp(Token::OF)) {
      ++i;
      expressionLevel9();
      fill = popExpression();
    } else {
      fill = ExpressionUnit::getSingleton();
    }

    do {
      Co<Expression> dimension = dimensions.top();
      dimensions.pop();
      Co<Expression> tmp = Co<Expression>(new ExpressionArrayConstructor(fill, dimension));
      SourceLocation location(dimension->getSourceLocation(), fill->getSourceLocation());
      tmp->setSource(getSubsource(location), location);
      fill = tmp;
    } while (dimensions.size() > 0);

    expressions.push(fill);
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 9: unary -.
void Parser::expressionLevel9() {
  if (isUp(Token::MINUS)) {
    ++i;
    expressionLevel9();
    Co<Expression> e = popExpression();
    pushExpression(new ExpressionNegation(e), e->getSourceLocation(), e->getSourceLocation());
  } else {
    expressionLevel10();
  }
}

/* ------------------------------------------------------------------------- */

// Precedence level 10: subscript [].
void Parser::expressionLevel10() {
  // Grab the simplest expression first.
  atom();

  // If we see a [, then that simplest expression is the base of a
  // subscript operation.
  while (isUp(Token::LEFT_BRACKET)) {
    ++i;
    Co<Expression> base = popExpression();

    expressionLevel0();
    Co<Expression> subscript = popExpression();

    // if ]
    // else if ..
    
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
        pushExpression(new ExpressionDefineElement(base, subscript, rhs), base->getSourceLocation(), rhs->getSourceLocation());
      } else {
        pushExpression(new ExpressionSubscript(base, subscript), base->getSourceLocation(), subscript->getSourceLocation());
      }
    }

    else if (isUp(Token::RANGE)) {
      ++i;

      expressionLevel0();
      Co<Expression> subscript2 = popExpression();

      if (isUp(Token::RIGHT_BRACKET)) {
        ++i;
        pushExpression(new ExpressionSubrange(base, subscript, subscript2), base->getSourceLocation(), subscript2->getSourceLocation());
      } else {
        std::stringstream ss;
        ss << subscript->getSourceLocation().toAnchor() << ": I didn't find a ']' where I expected one.";
        throw MessagedException(ss.str());
      }
    }
    
    else {
      std::stringstream ss;
      ss << subscript->getSourceLocation().toAnchor() << ": I didn't find a ']' where I expected one.";
      throw MessagedException(ss.str());
    }
  }
}

/* ------------------------------------------------------------------------- */

void Parser::atom() {
  if (isUp(Token::LEFT_PARENTHESIS)) {
    SourceLocation start_location = tokens[i].getLocation();
    ++i;
    expressionLevel0();
    Co<Expression> e = popExpression();
    if (isUp(Token::RIGHT_PARENTHESIS)) {
      SourceLocation location(start_location, tokens[i].getLocation());
      ++i;
      e->setSource(getSubsource(location), location);
      expressions.push(e);
    } else if (isUp(Token::COMMA)) {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found a comma where I did not expect one.";
      throw MessagedException(ss.str());
    } else {
      std::stringstream ss;
      ss << e->getSourceLocation().toAnchor() << ": I didn't find a ')' where I expected it.";
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
      ss << e->getSourceLocation().toAnchor() << ": I didn't find a closing '|' where I expected it.";
      throw MessagedException(ss.str());
    }
  } else if (isUp(Token::INTEGER)) {
    int n = atoi(tokens[i].getText().c_str());
    expressions.push(Co<Expression>(new ExpressionInteger(n)));
    expressions.top()->setSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::REAL)) {
    float n = atof(tokens[i].getText().c_str());
    expressions.push(Co<Expression>(new ExpressionReal(n)));
    expressions.top()->setSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::TRUE)) {
    expressions.push(Co<Expression>(new ExpressionBoolean(true)));
    expressions.top()->setSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::FALSE)) {
    expressions.push(Co<Expression>(new ExpressionBoolean(false)));
    expressions.top()->setSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::UNKNOWN)) {
    expressions.push(Co<Expression>(new ExpressionUnknown()));
    expressions.top()->setSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::AT_SIGN)) {
    ++i;
    if (isUp(Token::ID)) {
      expressions.push(Co<Expression>(new ExpressionIdentifier(tokens[i].getText())));
      expressions.top()->setSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
      ++i;
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I expected an identifier after @.";
      throw MessagedException(ss.str());
    }
  } else if (isUp(Token::NOTHING)) {
    expressions.push(ExpressionUnit::getSingleton());
    expressions.top()->setSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::STRING)) {
    expressions.push(Co<Expression>(new ExpressionString(tokens[i].getText())));
    expressions.top()->setSource(getSubsource(tokens[i].getLocation()), tokens[i].getLocation());
    ++i;
  } else if (isUp(Token::REPEAT)) {
    Token repeat_token = tokens[i];
    ++i;

    expressionLevel0();
    Co<Expression> n = popExpression();

    if (isUp(Token::NEWLINE)) {
      ++i;
      block();
      if (isUp(Token::AROUND)) {
        ++i;
        block();
        if (isUp(Token::END)) {
          /* Co<Expression> n = popExpression(); */
          Co<ExpressionBlock> less = popBlock();
          Co<ExpressionBlock> more = popBlock();
          pushExpression(new ExpressionRepeatwich(n, more, less), repeat_token.getLocation(), tokens[i].getLocation());
          ++i;
        } else {
          std::stringstream ss;
          ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
          throw MessagedException(ss.str());
        }
      } else if (isUp(Token::END)) {
        /* Co<Expression> n = popExpression(); */
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

    bool is_ternary = false;

    // One-liner.
    if (isUp(Token::THEN)) {
      is_ternary = true;
      ++i;
      expressionLevel0();
      Co<ExpressionBlock> then_block(new ExpressionBlock());
      Co<Expression> then_expression = popExpression();
      then_block->append(then_expression);
      then_block->setSource(then_expression->getSource(), then_expression->getSourceLocation());
      blocks.push(then_block);

      if (isUp(Token::ELSE)) {
        ++i;
        expressionLevel0();
        Co<ExpressionBlock> else_block(new ExpressionBlock());
        Co<Expression> else_expression = popExpression();
        else_block->append(else_expression);
        else_block->setSource(else_expression->getSource(), else_expression->getSourceLocation());
        blocks.push(else_block);
        end_location = else_block->getSourceLocation();
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
      end_location = blocks.top()->getSourceLocation();
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
        } else if (isUp(Token::IF)) {
          block();
          --i; // unget the linebreak of the nested if, which will be needed to close off this statement
          end_location = tokens[i].getLocation();
        } else {
          std::stringstream ss;
          ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a linebreak.";
          throw MessagedException(ss.str());
        }
      } else if (isUp(Token::END)) {
        end_location = tokens[i].getLocation();
        ++i;
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
    pushExpression(new ExpressionIf(condition, then_block, else_block, is_ternary), if_token.getLocation(), end_location);
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
        if (isUp(Token::NEWLINE, 2)) {
          std::stringstream ss;
          ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a single-line function definition, but I couldn't find a definition on its right-hand side.";
          throw MessagedException(ss.str());
        }
        ++i;
        expressionLevel0();
        rhs = popExpression();
        end_location = rhs->getSourceLocation();
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
        define->addFormal(*formal);
      }
      SourceLocation location(to_token.getLocation(), end_location);
      define->setSource(getSubsource(location), location);
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
      end_location = rhs->getSourceLocation();
    }

    Co<ExpressionDefineVariable> define;
    if (name == "seed") {
      pushExpression(new ExpressionDefineVariableSeed(name.c_str(), rhs), id_token.getLocation(), end_location);
    /* } else if (name == ".radius") { */
      /* pushExpression(new ExpressionDefineVariableRadius(name.c_str(), rhs), id_token.getLocation(), end_location); */
    } else {
      pushExpression(new ExpressionDefineVariable(name.c_str(), rhs), id_token.getLocation(), end_location);
    }
  } else if (isUp(Token::ID)) {
    Token id_token = tokens[i];
    SourceLocation end_location = id_token.getLocation();
    std::string name = tokens[i].getText();
    ++i;
    if (!isInExpressionFirst() || (isUp(Token::ID) && isUp(Token::COLON, 2))) {
      Co<ExpressionCallWithNamedParameters> call(new ExpressionCallWithNamedParameters(name));
      while (isUp(Token::ID) && isUp(Token::COLON, 2)) {
        std::string formal = tokens[i].getText();
        i += 2;
        expressionLevel0();
        Co<Expression> actual = popExpression();
        end_location = actual->getSourceLocation();
        call->addParameter(formal, actual);
      }
      SourceLocation location(id_token.getLocation(), end_location);
      call->setSource(getSubsource(location), location);
      expressions.push(call);
    } else {

      if (!isUp(Token::MINUS) && isInExpressionFirst()) {
        Co<ExpressionCall> call(new ExpressionCall(name));
        std::vector<Co<Expression> > actuals;

        expressionLevel0();
        Co<Expression> actual = popExpression();
        actuals.push_back(actual);
        call->addParameter(actual);
        end_location = actual->getSourceLocation();
        while (isUp(Token::COMMA)) {
          ++i;
          if (isInExpressionFirst()) {
            expressionLevel0();
            actual = popExpression();
            call->addParameter(actual);
            end_location = actual->getSourceLocation();
          } else {
            std::stringstream ss;
            ss << tokens[i - 1].getLocation().toAnchor() << ": I found an extra comma lying around.";
            throw MessagedException(ss.str());
          }
        }

        SourceLocation location(id_token.getLocation(), end_location);
        call->setSource(getSubsource(location), location);
        expressions.push(call);
      } else if (isUp(Token::MINUS)) {
        ++i; // skip past -
        expressionLevel0();
        Co<Expression> e = popExpression();

        // If we see a comma, then the minus should be applied to the first parameter and we have a call.
        if (isUp(Token::COMMA)) {
          Co<ExpressionCall> call(new ExpressionCall(name));
          std::vector<Co<Expression> > actuals;

          Co<Expression> actual(new ExpressionNegation(e));
          call->addParameter(actual);
          
          end_location = actual->getSourceLocation();
          while (isUp(Token::COMMA)) {
            ++i;
            if (isInExpressionFirst()) {
              expressionLevel0();
              actual = popExpression();
              call->addParameter(actual);
              end_location = actual->getSourceLocation();
            } else {
              std::stringstream ss;
              ss << tokens[i - 1].getLocation().toAnchor() << ": I found an extra comma lying around.";
              throw MessagedException(ss.str());
            }
          }

          SourceLocation location(id_token.getLocation(), end_location);
          call->setSource(getSubsource(location), location);
          expressions.push(call);
        } else {
          expressions.push(Co<Expression>(new ExpressionDynamicMinus(name, e)));
        }

        // yaw - foo
        // if has a comma, definitely a call
        // if no comma, create an ambiguous expr that's either a call or a subtraction
      }

      /* std::cout << "just saw call" << std::endl; */
      /* std::cout << isUp(Token::ASSIGN) << std::endl; */

      /* if (isUp(Token::ASSIGN)) { */
        /* ++i; */
        /* Co<Expression> rhs; */
        /* if (isUp(Token::NEWLINE)) { */
          /* ++i; */
          /* block(); */
          /* rhs = popBlock(); */
          /* if (isUp(Token::END)) { */
            /* ++i; */
          /* } else { */
            /* std::stringstream ss; */
            /* ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'."; */
            /* throw MessagedException(ss.str()); */
          /* } */
        /* } else { */
          /* expressionLevel0(); */
          /* rhs = popExpression(); */
        /* } */
        /* pushExpression(new ExpressionDefineElement(call, rhs), call->getSourceLocation(), rhs->getSourceLocation()); */
      /* } else { */
        /* pushExpression(new ExpressionArraySubscript(array, subscript), array->getSourceLocation(), subscript->getSourceLocation()); */
        /* expressions.push(call); */
      /* } */
    }
  } else if (isUp(Token::FOR)) {
    Token for_token = tokens[i];
    ++i;

    /* if (isUp(Token::ID)) { */
      /* std::string iterator_id = tokens[i].getText(); */

      is_in_loop_range = true;
      expressionLevel0();
      is_in_loop_range = false;

      Co<Expression> iterator = popExpression();

      Co<Expression> start;
      Co<Expression> stop;
      Co<Expression> by;
      Co<ExpressionBlock> body;
      bool is_inclusive = true;

      if (isUp(Token::TO)) {
        ++i;
        is_inclusive = false;
        is_in_loop_range = true;
        expressionLevel0();
        is_in_loop_range = false;
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
        is_in_loop_range = true;
        expressionLevel0();
        is_in_loop_range = false;
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
          pushExpression(new ExpressionFor(iterator, start, stop, by, body, is_inclusive), for_token.getLocation(), tokens[i].getLocation());
          ++i;
        } else {
          std::stringstream ss;
          ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected 'end'.";
          throw MessagedException(ss.str());
        }
      } else {
        expressionLevel0();
        Co<Expression> body = popExpression();
        pushExpression(new ExpressionFor(iterator, start, stop, by, body, is_inclusive), for_token.getLocation(), body->getSourceLocation());
      }
    /* } else { */
      /* std::stringstream ss; */
      /* ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected a name."; */
      /* throw MessagedException(ss.str()); */
    /* } */

  } else if (isUp(Token::LEFT_CURLY_BRACE)) {
    std::vector<Co<Expression> > items;
    Token left_bracket_token = tokens[i];

    if (!isUp(Token::RIGHT_CURLY_BRACE)) {
      do {
        ++i; // consume { or ,
        expressionLevel0();
        items.push_back(popExpression());
      } while (isUp(Token::COMMA));
    }

    if (isUp(Token::RIGHT_CURLY_BRACE)) {
      ExpressionArrayLiteral *array_literal = new ExpressionArrayLiteral(items);
      SourceLocation location(left_bracket_token.getLocation(), tokens[i].getLocation());
      pushExpression(array_literal, left_bracket_token.getLocation(), tokens[i].getLocation());
      ++i;
    } else {
      std::stringstream ss;
      ss << tokens[i].getLocation().toAnchor() << ": I found " << tokens[i].getQuotedText() << " in a place where I expected }.";
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
