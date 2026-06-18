// EC327 L15 activity -- TRIAGE SNIPPET 1 of 3.  THIS IS SUPPOSED TO FAIL.
// Build: g++ -std=c++20 -Wall -Wextra triage1_sort.cpp -o t1
//
// Your job: run the triage routine on the error wall.
//   1. Find YOUR line (not the <bits/...> plumbing).
//   2. Read the FIRST error as a sentence.
//   3. Name the missing capability, then make the smallest fix.
//
// Hint to check yourself: the wall mentions `operator<` and the type `Student`.

#include <algorithm>
#include <string>
#include <vector>

struct Student {
  std::string name;
  int gpa;
};

int main() {
  std::vector<Student> roster = {{"Ada", 95}, {"Bjarne", 88}, {"Grace", 99}};
  std::sort(roster.begin(), roster.end());  // <-- how should two Students compare?
  return 0;
}
