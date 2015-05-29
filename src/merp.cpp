#include <iostream>
#include <fstream>

#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/GeometryMode.h"
#include "madeup/Lexer.h"
#include "madeup/Parser.h"
#include "twodee/Co.h"
#include "twodee/MessagedException.h"

using namespace madeup;

int main(int argc, char **argv) {
  std::ifstream in(argv[1]);
  Lexer lexer(in);
  try {
    const std::vector<Token> &tokens = lexer.lex();
    Parser parser(tokens);
    Co<ExpressionBlock> program = parser.program();

    Environment env;
    env.Prime();
    env.SetGeometryMode(GeometryMode::SURFACE);
    program->Evaluate(env);

  } catch (td::MessagedException e) {
    std::cerr << e.GetMessage() << std::endl;
  }
  in.close();

  return 0;
}
