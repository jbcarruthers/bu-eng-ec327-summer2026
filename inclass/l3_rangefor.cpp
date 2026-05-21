// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L3 snippet -- the range-based for, in its three forms:
//          by value, by const reference, by writable reference.
// Topic: range-for
// Category: demo
// Lecture: L3
//
// Build & run:
//   g++ -std=c++20 -Wall l3_rangefor.cpp -o l3_rangefor
//   ./l3_rangefor

#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<int> paydays = {3, 8};   // the three-paycheck months

  // (1) BY VALUE -- m is a fresh copy of each element. Fine for ints.
  std::cout << "by value:\n";
  for (int m : paydays) {
    std::cout << "  month " << m << '\n';
  }

  // (2) BY CONST REFERENCE -- no copy, cannot modify. Use this for
  //     anything bigger than a number: strings, vectors, structs.
  std::vector<std::string> notes = {"payday", "rent due", "payday"};
  std::cout << "by const reference:\n";
  for (const std::string& note : notes) {
    std::cout << "  " << note << '\n';
  }

  // (3) BY WRITABLE REFERENCE -- change the elements in place.
  std::cout << "by reference (doubling each):\n";
  for (int& m : paydays) {
    m = m * 2;
  }
  for (int m : paydays) {
    std::cout << "  " << m << '\n';
  }

  // const auto& -- the safe default when you only need to read.
  std::cout << "const auto&: ";
  for (const auto& note : notes) {
    std::cout << note << ' ';
  }
  std::cout << '\n';
  return 0;
}
