#include "Log.h"

#include <cstdio>
#include <cstdarg>

namespace td {

/* ------------------------------------------------------------------------- */

void Log::Warn(const char *format,
               ...) {
  fprintf(stderr, "Warning: ");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
}

/* ------------------------------------------------------------------------- */

void Log::Error(const char *format,
                ...) {
  fprintf(stderr, "Error: ");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
}

/* ------------------------------------------------------------------------- */

void Log::Inform(const char *format,
                 ...) {
  fprintf(stderr, "Inform: ");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
}

/* ------------------------------------------------------------------------- */

void Log::Debug(const char *format,
                ...) {
  fprintf(stderr, "Debug: ");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
}

/* ------------------------------------------------------------------------- */

}
