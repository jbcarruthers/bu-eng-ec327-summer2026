#include "csv_writer.h"

#include <fstream>
#include <string>

#include "DataSet.h"

// =============================================================
//  csv_writer.cpp  --  Part 1. Implement write_csv().
//
//  Sketch:
//    - open an std::ofstream on `path`; if (!out) return false;
//    - write the header line: the kColumnNames joined by commas, then '\n';
//    - for each row, write x, y, r_squared, distance separated by commas;
//    - return true.
//
//  Two things to get right (see the handout's Hints):
//    1. PRECISION. The default ostream precision is 6 significant digits,
//       which silently rounds your data. Set enough precision that a value
//       round-trips exactly -- look up std::setprecision and the precision a
//       double needs.
//    2. NO TRAILING COMMA, and a clean line ending after the last column.
//       pandas is forgiving; numpy.loadtxt is not.
// =============================================================

bool write_csv([[maybe_unused]] const std::string& path,
               [[maybe_unused]] const DataSet& data) {
  // Part 1 -- implement me.
  return false;
}
