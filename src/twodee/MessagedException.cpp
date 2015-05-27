#include <iostream>

#include "twodee/MessagedException.h"

namespace td {

/* ------------------------------------------------------------------------- */

MessagedException::MessagedException(const std::string& message) throw()
  : message(message) {
}

/* ------------------------------------------------------------------------- */

MessagedException::~MessagedException() throw() {
}

/* ------------------------------------------------------------------------- */

const char *MessagedException::what() const throw() {
  return message.c_str(); 
}

/* ------------------------------------------------------------------------- */

const std::string& MessagedException::GetMessage() const {
  return message;
}

/* ------------------------------------------------------------------------- */

}
