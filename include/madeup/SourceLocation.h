#ifndef SOURCELOCATION_H
#define SOURCELOCATION_H

namespace madeup {

/* ------------------------------------------------------------------------- */

class SourceLocation {
  public:
    SourceLocation(int start_row,
                   int start_column,
                   int end_row,
                   int end_column);

    int getStartRow() const;
    int getStartColumn() const;
    int getEndRow() const;
    int getEndColumn() const;

  private:
    int start_row;
    int start_column;
    int end_row;
    int end_column;
};

/* ------------------------------------------------------------------------- */

}

#endif
