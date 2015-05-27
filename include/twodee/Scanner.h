#ifndef SCANNER_H
#define SCANNER_H

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A simplified clone of Java's Scanner. Parses lines and whitespace-delimited
 tokens from strings and files.
 */
class Scanner {
  public:
    /** Source types */
    enum source_t {
      SOURCE_FILE,
      SOURCE_STRING,
      SOURCE_STDIN
    };

    /**
     Creates a Scanner from the specified source.
     @param src Path to file if mode is <code>SOURCE_FILE</code>, text to parse if mode is
     <code>SOURCE_STRING</code>
     @param mode
     Type of input
     */
    Scanner(const std::string& src, source_t mode);

    /**
     Releases resources owned by this Scanner.
     */
    ~Scanner();

    /**
     If Scanner is parsing a file, the file is closed. It does nothing if
     parsing text.
     */
    void Close();

    /**
     Records the current "cursor" or seek pointer position so that the Scanner
     can return to it later.
     @see Rewind
     */
    void Mark();

    /** 
     Returns the Scanner to the previously marked position. If no position has
     been marked, the Scanner starts parsing at the beginning of the input
     source.
     */
    void Rewind();

    /**
     Checks if an integer can be read next.
     @returns True if an int is available, false otherwise
     */
    bool HasNextInt();

    /**
     Reads an int from the input source. Assumes an int is available.
     @returns Parsed int
     */
    int NextInt();

    /**
     Checks if a double can be read next.
     @returns True if a double is available, false otherwise
     */
    bool HasNextDouble();

    /**
     Reads a double from the input source. Assumes a double is available.
     @returns Parsed double
     */
    double NextDouble();

    /**
     Checks if a float can be read next.
     @returns True if a float is available, false otherwise
     */
    bool HasNextFloat();

    /**
     Reads a float from the input source. Assumes a float is available.
     @returns Parsed float
     */
    float NextFloat();

    /**
     Checks if a string can be read next.
     @returns True if a string is available, false otherwise
     */
    bool HasNext();

    /**
     Reads a string from the input source. Assumes a string is available.
     @returns Parsed string
     */
    std::string Next();

    /**
     Checks if a line of text can be read next.
     @returns True if a line is available, false otherwise
     */
    bool HasNextLine();

    /**
     Reads a line from the input source. Assumes a line is available.
     @returns Parsed line
     */
    std::string NextLine();

  private:
    /** Recorded seek pointer */
    std::streampos bookmark;

    /** Input stream */
    std::istream *in;
};

/* ------------------------------------------------------------------------- */

}

#endif
