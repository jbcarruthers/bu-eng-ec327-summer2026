// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L3 snippet -- why the index inside [] deserves a careful
//          eye. Two ordinary bugs, both reached with operator[], which
//          does not check the index: an off-by-one reads silent
//          garbage, an empty-vector access crashes outright -- no
//          message either way. Compare with l3_bug_at.cpp.
// Topic: at-vs-brackets
// Category: demo
// Lecture: L3
//
// Build & run:
//   g++ -std=c++20 -Wall l3_bug_brackets.cpp -o l3_bug_brackets
//   ./l3_bug_brackets ; echo "exit status: $?"

#include <iostream>
#include <vector>

int main() {
  // Paychecks counted in each of the 12 months. Valid indices: 0..11.
  std::vector<int> paydays = {2, 2, 3, 2, 2, 2, 2, 3, 2, 2, 2, 2};

  // ---- BUG 1: off-by-one ------------------------------------------
  // Month numbers run 1..12; month M lives at index M - 1. Forget the
  // "- 1" and month 12 reads paydays[12] -- one slot past the end.
  int month = 12;
  std::cout << "BUG 1: off-by-one, reading paydays[" << month << "]\n";
  std::cout << "  count = " << paydays[month] << '\n';   // [] : no check
  std::cout << "  ...still running. operator[] did not crash and did\n"
            << "  not complain -- it just handed back whatever sat in\n"
            << "  memory past the end of the vector. Worse, that value\n"
            << "  can look like a real paycheck count. The program\n"
            << "  exits 0, looking perfectly healthy. That is the trap."
            << std::endl;

  // ---- BUG 2: indexing a vector that turned out empty -------------
  // `hits` is meant to hold the three-paycheck months. Suppose a bug
  // left it EMPTY -- and we read hits[0] anyway, without ever checking
  // hits.empty() first.
  std::vector<int> hits;                  // empty: zero elements
  std::cout << "\nBUG 2: empty vector, reading hits[0] ..."
            << std::endl;                 // flush BEFORE the crash
  int first = hits[0];                    // [] on empty: SEGFAULT here
  std::cout << "  first three-paycheck month = " << first << '\n';
  std::cout << "  (you will never see this line)\n";

  return 0;
}
