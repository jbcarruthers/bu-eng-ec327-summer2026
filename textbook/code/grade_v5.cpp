// grade_v5.cpp -- EC327 Summer 2026
//
// Stage 5: notice that "the threshold and the letter that goes with it"
// are *one* idea, awkwardly split across two parallel vectors. Pull them
// together into a small struct, and iterate over a single vector<Grade>
// using a range-based for loop with an early break.
//
// This is the version that finally *looks like* what the problem is
// actually asking for: "find the first row of this table whose threshold
// I'm below, and use that row's letter."

#include <iostream>
#include <vector>

struct Grade {
  double threshold;
  char letter;
};

int main() {
  std::vector<Grade> scale{
      {50.0, 'F'},
      {60.0, 'D'},
      {70.0, 'C'},
      {80.0, 'B'},
      {100.0, 'A'},
  };

  double g{};
  std::cout << "Enter the student's grade: ";
  std::cin >> g;

  char let = '?';
  for (const auto& row : scale) {
    let = row.letter;
    if (g < row.threshold) {
      break;
    }
  }

  std::cout << "The letter grade is " << let << ".\n";
  return 0;
}
