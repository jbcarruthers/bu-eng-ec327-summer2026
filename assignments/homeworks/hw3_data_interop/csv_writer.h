#pragma once

#include <string>

#include "DataSet.h"

// =============================================================
//  csv_writer.h  --  EC327 Summer 2026, Homework 3, Part 1
//
//  CSV is the universal interchange format: every spreadsheet, every
//  data-science library, every database can read it. It is also a TEXT
//  format you can write by hand with nothing but <fstream> and <iomanip> --
//  no library required. That is the point of Part 1: write data that a
//  pandas.read_csv() / numpy.loadtxt() call on the other side loads cleanly.
//
//  The body lives in csv_writer.cpp and is empty. Implement it.
// =============================================================

// Write the table to `path` as CSV. The first line is the header -- the four
// column names from kColumnNames, comma-separated. Each following line is one
// WalkRecord: x, y, r_squared, distance, comma-separated, in that order.
//
// Returns true on success, false if the file could not be opened for writing.
// The caller checks the return value -- a writer that silently fails to open
// its file is a classic interoperability bug.
bool write_csv(const std::string& path, const DataSet& data);
