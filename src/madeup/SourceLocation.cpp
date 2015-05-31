#include <iostream>
#include <sstream>

#include "madeup/SourceLocation.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

SourceLocation::SourceLocation() :
  start_row(-1),
  start_column(-1),
  end_row(-1),
  end_column(-1),
  start_index(-1),
  end_index(-1) {
}

/* ------------------------------------------------------------------------- */

SourceLocation::SourceLocation(const SourceLocation &from,
                               const SourceLocation &to) :
  start_row(from.start_row),
  start_column(from.start_column),
  end_row(to.end_row),
  end_column(to.end_column),
  start_index(from.start_index),
  end_index(to.end_index) {
}

/* ------------------------------------------------------------------------- */

SourceLocation::SourceLocation(int start_row,
                               int start_column,
                               int end_row,
                               int end_column,
                               int start_index,
                               int end_index) :
  start_row(start_row),
  start_column(start_column),
  end_row(end_row),
  end_column(end_column),
  start_index(start_index),
  end_index(end_index) {
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

int SourceLocation::getStartIndex() const {
  return start_index;
}

/* ------------------------------------------------------------------------- */

int SourceLocation::getEndIndex() const {
  return end_index;
}

/* ------------------------------------------------------------------------- */

std::string SourceLocation::toAnchor() const {
  std::stringstream ss;
  ss << start_row << "(" << start_index << "-" << end_index << ")";
  return ss.str(); 
}

/* ------------------------------------------------------------------------- */

}
