#pragma once

#include <vector>

// =============================================================
//  DataSet.h  --  EC327 Summer 2026, Homework 3
//
//  The thing you are going to serialize. A DataSet is a table: K rows, one
//  per finished walker, and four columns. Every column is a double so the
//  whole table is one homogeneous block of numbers -- which is exactly what
//  makes the binary .npy format in Part 2 simple to write.
//
//  This file is PROVIDED COMPLETE. generate_dataset() (in DataSet.cpp) runs
//  the HW2 simulation and fills the table. Your job is to write that table
//  out as CSV (Part 1) and as a NumPy .npy array (Part 2).
// =============================================================

// One row of the table -- one finished walker. Column order is fixed:
//   x, y, r_squared, distance
// and the writers MUST emit the columns in this order so the CSV header and
// the .npy array agree with what the Python loader expects.
struct WalkRecord {
  double x;          // final x-coordinate
  double y;          // final y-coordinate
  double r_squared;  // x*x + y*y
  double distance;   // sqrt(x*x + y*y)
};

// The table: K rows of four columns each.
using DataSet = std::vector<WalkRecord>;

// The column names, in order, for the CSV header row and for Python's
// DataFrame columns. Keep your CSV header identical to this.
inline constexpr const char* kColumnNames[] = {"x", "y", "r_squared",
                                               "distance"};
inline constexpr int kNumColumns = 4;

// Run K independent walkers for N steps each (HW2's simulation) and pack the
// finished positions into a DataSet. Seeded for reproducibility: the same
// seed gives the same table every run, which is what lets the test suite
// check the numbers. Provided complete -- you do not edit this.
DataSet generate_dataset(int K, int N, unsigned seed);
