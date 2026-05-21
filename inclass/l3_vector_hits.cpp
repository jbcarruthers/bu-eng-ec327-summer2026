// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L3 snippet -- std::vector, the growable list. Collect the
//          three-paycheck months without knowing how many there are.
// Topic: vector
// Category: demo
// Lecture: L3
//
// Build & run:
//   g++ -std=c++20 -Wall l3_vector_hits.cpp -o l3_vector_hits
//   ./l3_vector_hits

#include <iostream>
#include <vector>

int main() {
  // Paychecks counted in each of the 12 months, for one example pay
  // schedule. Most months get 2; a rare few get 3.
  std::vector<int> monthly = {2, 2, 3, 2, 2, 2, 2, 3, 2, 2, 2, 2};

  // Which months got three? We don't know how many until we look --
  // so we collect them in a vector that grows as we push onto it.
  std::vector<int> hits;                 // empty, growable list of ints

  for (int month = 1; month <= 12; month += 1) {
    if (monthly[month - 1] >= 3) {       // month M lives at index M - 1
      hits.push_back(month);             // append -- the vector grows
    }
  }

  std::cout << "checked " << monthly.size() << " months\n";
  std::cout << "three-paycheck months: " << hits.size() << '\n';
  for (int m : hits) {
    std::cout << "  month " << m << '\n';
  }
  return 0;
}
