// grade_v1.cpp -- EC327 Summer 2026
//
// Calculate a letter grade from a numerical score, version 1.
//
// Grading table:
//   < 50  F
//  50-59  D
//  60-69  C
//  70-79  B
//   >=80  A
//
// Stage 1: a sequence of independent if-statements, one per range.
// This *looks* like it works. It has two real bugs we will catch with
// stage 2. (Do you see them yet?)

#include <iostream>

int main() {
  double g{};
  char let{};

  std::cout << "Enter the student's grade: ";
  std::cin >> g;

  if (g < 50.0) {
    let = 'F';
  }
  if (g >= 50.0 and g < 60.0) {
    let = 'D';
  }
  if (g >= 60.0 and g < 70.0) {
    let = 'C';
  }
  if (g >= 70.0 and g < 80.0) {
    let = 'B';
  }
  if (g > 80.0) {  // BUG: should be g >= 80.0
    let = 'A';
  }

  std::cout << "The letter grade is " << let << ".\n";
  return 0;
}
