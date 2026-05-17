// grade_v2.cpp -- EC327 Summer 2026
//
// Calculate a letter grade from a numerical score, version 2.
//
// Grading table:
//   < 50  F
//  50-59  D
//  60-69  C
//  70-79  B
//   >=80  A
//
// Stage 2: a single if / else if / else chain. Fixes both stage-1 bugs:
//   1. The boundary error at 80 (strict > vs >=) is gone, because
//      "g < 80.0" handles 79.999 and the trailing `else` handles 80+.
//   2. The "uninitialized `let` if no branch matches" failure mode is gone,
//      because the `else` is unconditional. The chain is exhaustive.

#include <iostream>

int main() {
  double g{};
  char let{};

  std::cout << "Enter the student's grade: ";
  std::cin >> g;

  if (g < 50.0) {
    let = 'F';
  } else if (g < 60.0) {
    let = 'D';
  } else if (g < 70.0) {
    let = 'C';
  } else if (g < 80.0) {
    let = 'B';
  } else {
    let = 'A';
  }

  std::cout << "The letter grade is " << let << ".\n";
  return 0;
}
