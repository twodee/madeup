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

void usage(const std::string &message) {
  std::cerr << "Usage: merp [-r] [--timeout #] [--tree] -i in.mup [-o out.obj] [--geometry (SURFACE|PATH|NONE)]" << std::endl;
  std::cerr << message << std::endl;
  exit(1);
}

int main(int argc, char **argv) {
  bool wants_tree = false;
  bool wants_tokens = false;
  bool wants_timeout = false;
  bool wants_render = false;
  int timeout = 45;
  std::string out_path = "";
  GeometryMode::geometry_mode_t geometry_mode = GeometryMode::SURFACE;

  int optind = 1;
  for (; optind < argc && argv[optind][0] == '-'; ++optind) {
    std::string formal = argv[optind];

    if (formal == "--timeout") {
      wants_timeout = true;
      if (optind < argc - 1) {
        for (int i = 0; i < strlen(argv[optind + 1]); ++i) {
          if (!isdigit(argv[optind + 1][i])) {
            usage("Timeout must be positive integer.");
          }
        }
        timeout = atoi(argv[optind + 1]);
        ++optind;
      } else {
        usage("Timeout must be given.");
      }
    }

    else if (formal == "--tree") {
      wants_tree = true;
    }
    
    else if (formal == "--tokens") {
      wants_tokens = true;
    }

    else if (formal == "-r") {
      wants_render = true;
    }

    else if (formal == "-o") {
      if (optind < argc - 1) {
        out_path = argv[optind + 1];
        ++optind;
      } else {
        usage("-o given, but no path specified.");
      }
    }

    else if (formal == "--geometry") {
      if (optind < argc - 1) {
        std::string mode_label = argv[optind + 1];
        if (mode_label == "SURFACE") {
          geometry_mode = GeometryMode::SURFACE;
        } else if (mode_label == "PATH") {
          geometry_mode = GeometryMode::PATH;
        } else if (mode_label == "NONE") {
          geometry_mode = GeometryMode::NONE;
        } else {
          usage("unknown geometry mode");
        }
        ++optind;
      } else {
        usage("--geometry given, but no path specified.");
      }
    }

    else {
      usage("unknown option: " + formal);
    }
  }

  if (optind >= argc) {
    usage("No input path specified.");
  }

  std::string in_path = argv[optind];
  ++optind;

  if (optind < argc) {
    usage("Stray parameters.");
  }

  std::string source = Utilities::Slurp(in_path);

  std::ifstream in(in_path);
  Lexer lexer(in);
  try {
    const std::vector<Token> &tokens = lexer.lex();

    if (wants_tokens) {
      for (std::vector<Token>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) {
        std::cout << *i << std::endl;
      }
    }
    
    Parser parser(tokens, source);
    Co<ExpressionBlock> program = parser.program();

    if (wants_tree) {
      std::cout << program << std::endl;
    }

    Environment env;
    env.Prime();
    env.SetGeometryMode(geometry_mode);
    srand(time(NULL));

    program->Evaluate(env);

    Trimesh *trimesh = env.GetMesh();

    if (out_path.length() > 0) {
      if (geometry_mode == GeometryMode::PATH) {
        std::ofstream out(out_path.c_str());
        out << env.GetPathsJSON();
        out.close();
      } else if (geometry_mode == GeometryMode::SURFACE) {
        if (out_path.find_first_of('.') == std::string::npos ||
            out_path.find(".json") == out_path.length() - 5) {
          trimesh->WriteJSON(out_path);
        } else {
          trimesh->WriteObj(out_path);
        }
      }

      if (wants_render) {
        std::string command = "runx /Users/johnch/checkouts/cj_graphics/build/projects/mesher/mesher.app -m " + out_path;
        system(command.c_str());
      }
    }

  } catch (td::MessagedException e) {
    std::cerr << e.GetMessage() << std::endl;
    exit(1);
  }
  in.close();

  return 0;
}
