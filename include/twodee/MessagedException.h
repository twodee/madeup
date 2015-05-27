#ifndef MESSAGEDEXCEPTION_H
#define MESSAGEDEXCEPTION_H

#include <exception>
#include <string>

namespace td {

/* ------------------------------------------------------------------------- */

/**
 An exception with an explanatory message.
 */
class MessagedException : public std::exception {
  public:
    /**
     Creates a new exception with the specified message.
     @param message explaining the cause of the exception.
     */
    MessagedException(const std::string& message) throw();

    /**
     Frees the resources consumed by this exception.
     */
    virtual ~MessagedException() throw();

    /**
     Gets the explanatory message.
     @return Explanatory message
     */
    const char *what() const throw();

    const std::string& GetMessage() const;

  private:
    /** The message explaining the cause of this exception */
    std::string message;
};

/* ------------------------------------------------------------------------- */

}

#endif
