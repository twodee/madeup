#ifndef LOG_H
#define LOG_H

#include <string>

namespace td {

/* ------------------------------------------------------------------------- */

/**
 An abstraction of a message logging system.
 */
class Log {
  public:
    /**
     Send a warning message to the log.
     @param format Format of message to show
     @param ... Parameters for format message
     */
    static void Warn(const char *format, ...);

    /**
     Send an error message to the log.
     @param format Format message to show
     @param ... Parameters for format message
     */
    static void Error(const char *format, ...);

    /**
     Send an informational message to the log.
     @param format Format message to show
     @param ... Parameters for format message
     */
    static void Inform(const char *format, ...);

    /**
     Send a debug message to the log.
     @param format Format message to show
     @param ... Parameters for format message
     */
    static void Debug(const char *format, ...);
};

/* ------------------------------------------------------------------------- */

}

#endif
