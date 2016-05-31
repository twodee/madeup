#include <fstream>

#include "twodee/MessagedException.h"
#include "Utilities.h"

namespace td {

/* ------------------------------------------------------------------------- */

char *Utilities::FileToString(const std::string& path) {
  std::ifstream f(path.c_str(), std::ios::in | std::ios::ate | std::ios::binary);

  if (f.fail()) {
    throw MessagedException("can't open file " + path + " for reading");
  }

  // Make buffer that's the size of the file.
  int size = (int) f.tellg();
  char *s = new char[size + 1];

  // Go back to file start and read contents in.
  f.seekg(0, std::ios::beg);
  f.read(s, size);
  f.close();

  // Null terminate.
  s[size] = '\0';

  return s;
}

/* ------------------------------------------------------------------------- */

bool Utilities::IsLittleEndian() {
  int test = 1;
  return ((char *) &test)[0] == 1;
}

/* ------------------------------------------------------------------------- */

void Utilities::ReverseEndian(void *p,
                              int nbytes) {
  char *pchar = (char *) p;
  for (int i = 0; i < nbytes / 2; ++i) {
    char tmp = pchar[i];
    pchar[i] = pchar[nbytes - 1 - i];
    pchar[nbytes - 1 - i] = tmp;
  }
}

/* ------------------------------------------------------------------------- */

bool Utilities::Replace(std::string& in,
                        const std::string& replacee,
                        const std::string& replacer) {
  size_t i = in.find(replacee);
  if (i == std::string::npos) {
    return false;
  } else {
    in.replace(i, replacee.length(), replacer);
    return true;
  }
}

/* ------------------------------------------------------------------------- */

}
