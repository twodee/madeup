#include <iostream>

#include "madeup/SourceLocation.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

SourceLocation::SourceLocation(int start_row,
                               int start_column,
                               int end_row,
                               int end_column) :
  start_row(start_row),
  start_column(start_column),
  end_row(end_row),
  end_column(end_column) {
}

/* ------------------------------------------------------------------------- */

int SourceLocation::getStartRow() const {
  return start_row;
}

/* ------------------------------------------------------------------------- */

int SourceLocation::getStartColumn() const {
  return start_column;
}

/* ------------------------------------------------------------------------- */

int SourceLocation::getEndRow() const {
  return end_row;
}

/* ------------------------------------------------------------------------- */

int SourceLocation::getEndColumn() const {
  return end_column;
}

/* ------------------------------------------------------------------------- */

}
