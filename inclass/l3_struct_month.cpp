// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L3 snippet -- defining your first struct. Bundle two facts
//          about a month into one named type, ending the two-arrays-
//          kept-in-step problem.
// Topic: struct
// Category: demo
// Lecture: L3
//
// Build & run:
//   g++ -std=c++20 -Wall l3_struct_month.cpp -o l3_struct_month
//   ./l3_struct_month

#include <iostream>
#include <vector>

// Your first defined type. A struct bundles related values -- here,
// two facts about one month -- into a single named type.
struct Month {
  int length;    // days in the month
  int paydays;   // paychecks counted in it
};

int main() {
  // One Month value holds both facts together; reach a field with a dot.
  Month march = {31, 3};
  std::cout << "March: " << march.length << " days, " << march.paydays
            << " paychecks\n";

  // The two-arrays-kept-in-step problem is gone: one vector, each
  // element a whole Month -- length and count can never drift apart.
  std::vector<Month> year = {
      {31, 2}, {28, 2}, {31, 3}, {30, 2}, {31, 2}, {30, 2},
      {31, 2}, {31, 3}, {30, 2}, {31, 2}, {30, 2}, {31, 2},
  };

  int month = 1;
  for (const Month& m : year) {
    // Structured bindings unpack a struct's fields, too.
    auto [length, paydays] = m;
    if (paydays >= 3) {
      std::cout << "month " << month << ": three paychecks, in a "
                << length << "-day month\n";
    }
    month += 1;
  }
  return 0;
}
