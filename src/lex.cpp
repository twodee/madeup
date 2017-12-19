#include <iostream>
#include <fstream>

#include "madeup/Lexer.h"
#include "twodee/MessagedException.h"

using namespace madeup;

int main(int argc, char **argv) {
  std::ifstream in(argv[1]);
  Lexer lexer(in);
  try {
    const std::vector<Token> &tokens = lexer.lex();
    for (std::vector<Token>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) {
      std::cout << *i << (i->isPrecededBySpace() ? " (follows space)" : "") << std::endl;
    }
  } catch (td::MessagedException e) {
    std::cerr << e.GetMessage() << std::endl;
  }
  in.close();

  return 0;
}
