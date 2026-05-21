// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L3 snippet -- the same two bugs as l3_bug_brackets.cpp, this
//          time reached with .at(). .at() checks the index: out of
//          range throws std::out_of_range -- one clean, named,
//          catchable failure, immediately, every time.
// Topic: at-vs-brackets
// Category: demo
// Lecture: L3
//
// Build & run:
//   g++ -std=c++20 -Wall l3_bug_at.cpp -o l3_bug_at
//   ./l3_bug_at ; echo "exit status: $?"

#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
  // Paychecks counted in each of the 12 months. Valid indices: 0..11.
  std::vector<int> paydays = {2, 2, 3, 2, 2, 2, 2, 3, 2, 2, 2, 2};

  // ---- BUG 1: off-by-one, now with .at() --------------------------
  int month = 12;
  std::cout << "BUG 1: off-by-one, reading paydays.at(" << month << ")\n";
  try {
    int count = paydays.at(month);
    std::cout << "  count = " << count << '\n';
  } catch (const std::out_of_range& e) {
    std::cout << "  caught std::out_of_range: " << e.what() << '\n';
    std::cout << "  -- BUG 1 was silent under []; .at() makes it loud,\n"
              << "  reported the instant the bad index is used.\n";
  }

  // ---- BUG 2: indexing a vector that turned out empty -------------
  std::vector<int> hits;                  // empty: zero elements
  std::cout << "\nBUG 2: empty vector, reading hits.at(0)\n";
  try {
    int first = hits.at(0);
    std::cout << "  first three-paycheck month = " << first << '\n';
  } catch (const std::out_of_range& e) {
    std::cout << "  caught std::out_of_range: " << e.what() << '\n';
    std::cout << "  -- .at() refuses an index into an empty vector\n"
              << "  instead of dereferencing a null pointer.\n";
  }

  std::cout << "\nProgram finished normally (exit status 0). Both bugs\n"
            << "were caught and reported. Uncaught, each would still\n"
            << "stop the program with that same message -- never a\n"
            << "silent wrong answer, never a bare segfault.\n";
  return 0;
}
