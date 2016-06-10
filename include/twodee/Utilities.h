#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

namespace td {

/* ------------------------------------------------------------------------- */

const double PI = 3.1415926535897932384626433832795;
const double E = 2.7182818284590452353602874713526;

enum {
  HSV,
  HSVA,
  RGB,
  RGBA
};

/* ------------------------------------------------------------------------- */

/**
 A container class for several standalone utility methods.
 */
class Utilities {
  public:
    /**
     Gets the contents of the specified file as a string.
     @param path Path to file
     @return String contents of file
     */
    static char *FileToString(const std::string& path);
    static std::string Slurp(const std::string& path);

    static int ToInt(const std::string &s);

    /**
     Convert array of one type to array of another. Assumes destination array
     has already been allocated.
     @param dst Preallocated destination array
     @param src Array from which to copy values
     @param nitems Number of items to copy
     */
    template<class T, class U>
    static void ConvertArray(U *dst,
                             const T *src,
                             int nitems);

    static bool IsLittleEndian();

    static void ReverseEndian(void *p,
                              int nbytes);

    static bool Replace(std::string& in,
                        const std::string& replacee,
                        const std::string& replacer);

    template<typename T> static int Sign(T value);
    template<typename T> static T ClampAbove(T value, T min);
    template<typename T> static T ClampBelow(T value, T max);
    template<typename T> static T ClampBetween(T value, T min, T max);
};

/* ------------------------------------------------------------------------- */

template<class T, class U>
void Utilities::ConvertArray(U *dst,
                             const T *src,
                             int nitems) {
  for (int i = 0; i < nitems; i++) {
    dst[i] = (U) src[i];
  }
}

/* ------------------------------------------------------------------------- */

template<typename T> int Utilities::Sign(T value) {
  if (value < 0) {
    return -1;
  } else if (value > 0) {
    return 1;
  } else {
    return 0;
  }
}

/* ------------------------------------------------------------------------- */

template<typename T> T Utilities::ClampAbove(T value, T min) {
  return value < min ? min : value;
}

/* ------------------------------------------------------------------------- */

template<typename T> T Utilities::ClampBelow(T value, T max) {
  return value > max ? max : value;
}

/* ------------------------------------------------------------------------- */

template<typename T> T Utilities::ClampBetween(T value, T min, T max) {
  return ClampAbove(ClampBelow(value, max), min);
}

/* ------------------------------------------------------------------------- */

}

#endif
