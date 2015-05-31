#ifndef SOURCELOCATION_H
#define SOURCELOCATION_H

namespace madeup {

/* ------------------------------------------------------------------------- */

struct SourceLocation {
  public:
    SourceLocation();
    SourceLocation(const SourceLocation &from,
                   const SourceLocation &to);
    SourceLocation(int start_row,
                   int start_column,
                   int end_row,
                   int end_column,
                   int start_index,
                   int end_index);

    int getStartRow() const;
    int getStartColumn() const;
    int getEndRow() const;
    int getEndColumn() const;
    int getStartIndex() const;
    int getEndIndex() const;

    std::string toAnchor() const;

  /* private: */
    int start_row;
    int start_column;
    int end_row;
    int end_column;
    int start_index;
    int end_index;
};

/* ------------------------------------------------------------------------- */

}

#endif
