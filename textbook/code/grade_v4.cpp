// grade_v4.cpp -- EC327 Summer 2026
//
// Stage 4: replace the C-style arrays with std::vector. The logic is
// identical; the only thing that changes is the *container*. This is the
// version you would actually write in 2026 -- C-style arrays are a
// historical hazard we use only to show what they don't give us.

#include <iostream>
#include <vector>

int main() {
  std::vector<double> thresholds{50.0, 60.0, 70.0, 80.0, 100.0};
  std::vector<char>   letters   {'F',  'D',  'C',  'B',  'A'};

  double g{};
  std::cout << "Enter the student's grade: ";
  std::cin >> g;

  std::size_t i = 0;
  while (g >= thresholds[i]) {
    i++;
  }
  char let = letters[i];

  std::cout << "The letter grade is " << let << ".\n";
  return 0;
}
