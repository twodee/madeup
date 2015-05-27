#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "twodee/MessagedException.h"
#include "twodee/Scanner.h"

namespace td {

/* ------------------------------------------------------------------------- */

Scanner::Scanner(const std::string& src, source_t mode) {
  if (mode == SOURCE_STRING) {
    in = new std::stringstream(src, std::stringstream::in);
  } else if (mode == SOURCE_FILE) {
    in = new std::ifstream(src.c_str());
  } else if (mode == SOURCE_STDIN) {
    in = &std::cin;
  } else {
    assert("bad Scanner mode");
    in = new std::ifstream("/dev/null");
  }
  Mark();
}

/* ------------------------------------------------------------------------- */

Scanner::~Scanner() {
  delete in;
}

/* ------------------------------------------------------------------------- */

void Scanner::Close() {
  std::ifstream *file = dynamic_cast<std::ifstream *>(in);
  if (file) {
    file->close();
  }
}

/* ------------------------------------------------------------------------- */

bool Scanner::HasNextInt() {
  std::streampos mark = in->tellg();

  // try to grab int
  int i;
  (*in) >> i;

  // record success
  bool ok = !in->fail();

  // ok is currently true even if the token was 56foo. 56 will be pulled off.
  // We want to ensure the entire token is an int. Let's peek and assert
  // whitespace.
  if (ok) {
    char c;
    in->get(c);
    if (!in->fail()) {
      ok = isspace(c);
    }
  }

  // return to mark
  in->clear();
  in->seekg(mark);

  return ok;
}

/* ------------------------------------------------------------------------- */

int Scanner::NextInt() {
  int i;
  (*in) >> i;
  if (in->fail()) {
    throw MessagedException("Int couldn't be read");
  }
  return i;  
}

/* ------------------------------------------------------------------------- */

bool Scanner::HasNextDouble() {
  std::streampos mark = in->tellg();

  // try to grab int
  double d;
  (*in) >> d;

  // record success
  bool ok = !in->fail();

  // ok is currently true even if the token was 56foo. 56 will be pulled off.
  // We want to ensure the entire token is an int. Let's peek and assert
  // whitespace.
  if (ok) {
    char c;
    in->get(c);
    if (!in->fail()) {
      ok = isspace(c);
    }
  }

  // return to mark
  in->clear();
  in->seekg(mark);

  return ok;
}

/* ------------------------------------------------------------------------- */

double Scanner::NextDouble() {
  double d;
  (*in) >> d;
  if (in->fail()) {
    throw MessagedException("Double couldn't be read");
  }
  return d;  
}

/* ------------------------------------------------------------------------- */

bool Scanner::HasNextFloat() {
  std::streampos mark = in->tellg();

  // try to grab int
  float f;
  (*in) >> f;

  // record success
  bool ok = !in->fail();

  // ok is currently true even if the token was 56foo. 56 will be pulled off.
  // We want to ensure the entire token is an int. Let's peek and assert
  // whitespace.
  if (ok) {
    char c;
    in->get(c);
    if (!in->fail()) {
      ok = isspace(c);
    }
  }

  // return to mark
  in->clear();
  in->seekg(mark);

  return ok;
}

/* ------------------------------------------------------------------------- */

float Scanner::NextFloat() {
  float f;
  (*in) >> f;
  if (in->fail()) {
    throw MessagedException("Float couldn't be read");
  }
  return f;  
}

/* ------------------------------------------------------------------------- */

bool Scanner::HasNext() {
  std::streampos mark = in->tellg();

  // try to grab string
  std::string str;
  (*in) >> str;

  // record success
  bool ok = !in->fail();

  // return to mark
  in->clear();
  in->seekg(mark);

  return ok;
}

/* ------------------------------------------------------------------------- */

std::string Scanner::Next() {
  std::string str;
  (*in) >> str;
  if (in->fail()) {
    throw MessagedException("String couldn't be read");
  }
  return str;
}

/* ------------------------------------------------------------------------- */

bool Scanner::HasNextLine() {
  std::streampos mark = in->tellg();
  std::string line;
  getline(*in, line);

  // record success
  bool ok = !in->fail();

  // return to mark
  in->clear();
  in->seekg(mark);

  return ok;
}

/* ------------------------------------------------------------------------- */

std::string Scanner::NextLine() {
  std::string line;
  getline(*in, line);
  if (in->fail()) {
    throw MessagedException("Line couldn't be read");
  }
  return line;
}

/* ------------------------------------------------------------------------- */

void Scanner::Mark() {
  bookmark = in->tellg();
}

/* ------------------------------------------------------------------------- */

void Scanner::Rewind() {
  in->clear(); // needed to clear error flags
  in->seekg(bookmark);
}

/* ------------------------------------------------------------------------- */

}
