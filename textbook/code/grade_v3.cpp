// grade_v3.cpp -- EC327 Summer 2026
//
// Stage 3: notice that the if/else if/else chain of grade_v2 is itself a
// repeated pattern (compare g against a threshold, assign the corresponding
// letter). Replace it with two parallel C-style arrays and a `while` loop
// that walks them.
//
// This is the FIRST time in the book that we use a collection, and the
// first time we use a loop other than the ones cin >> hidden inside its
// internal whitespace handling. We use C-style arrays here only because
// they show what a vector replaces; grade_v4 immediately moves to std::vector.

#include <iostream>

int main() {
  double thresholds[5] = {50.0, 60.0, 70.0, 80.0, 100.0};
  char letters[5] = {'F', 'D', 'C', 'B', 'A'};

  double g{};
  std::cout << "Enter the student's grade: ";
  std::cin >> g;

  int i = 0;
  while (g >= thresholds[i]) {
    i++;
  }
  char let = letters[i];

  std::cout << "The letter grade is " << let << ".\n";
  return 0;
}
