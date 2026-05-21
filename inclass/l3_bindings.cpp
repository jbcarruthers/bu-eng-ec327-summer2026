// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L3 snippet -- structured bindings. Unpack several values at
//          once, naming each part, instead of .first / .second.
// Topic: structured-bindings
// Category: demo
// Lecture: L3
//
// Build & run:
//   g++ -std=c++20 -Wall l3_bindings.cpp -o l3_bindings
//   ./l3_bindings

#include <array>
#include <iostream>
#include <utility>

int main() {
  // A "thing" that is really two values at once: a month and its count.
  std::pair<int, int> entry = {8, 3};

  // Structured binding: unpack both values, naming each part yourself.
  auto [month, count] = entry;
  std::cout << "month " << month << " got " << count << " paychecks\n";

  // Most powerful inside a range-based for over a list of pairs.
  std::array<std::pair<int, int>, 4> monthly = {{
      {1, 2}, {2, 2}, {3, 3}, {4, 2},
  }};

  for (auto [m, c] : monthly) {
    std::cout << "  month " << m << ": " << c << " paychecks";
    if (c >= 3) {
      std::cout << "  <-- three!";
    }
    std::cout << '\n';
  }
  return 0;
}
