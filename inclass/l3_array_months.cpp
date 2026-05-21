// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L3 snippet -- std::array, the fixed-size sibling of vector.
//          Twelve months: a count that never grows or shrinks.
// Topic: array
// Category: demo
// Lecture: L3
//
// Build & run:
//   g++ -std=c++20 -Wall l3_array_months.cpp -o l3_array_months
//   ./l3_array_months

#include <array>
#include <iostream>

int main() {
  // There are exactly 12 months -- always. A std::array has its size
  // baked in at compile time; unlike a vector, it cannot grow.
  std::array<int, 12> paydays = {2, 2, 3, 2, 2, 2, 2, 3, 2, 2, 2, 2};

  std::cout << "size: " << paydays.size() << " (fixed)\n";

  // Index from 0: month M lives at paydays[M - 1]. Watch the off-by-one
  // -- the index is the first place to look when something is wrong.
  std::cout << "March  (month 3) got " << paydays[2] << " paychecks\n";
  std::cout << "August (month 8) got " << paydays[7] << " paychecks\n";

  // range-based for works on std::array exactly as on std::vector.
  int month = 1;
  for (int count : paydays) {
    if (count >= 3) {
      std::cout << "month " << month << " is a three-paycheck month\n";
    }
    month += 1;
  }
  return 0;
}
