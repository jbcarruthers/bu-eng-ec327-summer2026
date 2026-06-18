// EC327 L16 activity -- guess the big-O.
// Build: g++ -std=c++20 -Wall -Wextra bigo_triage.cpp -o bigo && ./bigo
//
// Five functions.  For EACH, in your pair, write down its big-O in terms of
// the input size n (= v.size(), or the argument n) BEFORE you look at the
// answer.  Use the recognize-the-shape rule from lecture:
//   one pass over the data .......... O(n)
//   loop inside a loop, same data ... O(n^2)
//   halve the space each step ....... O(log n)
//   sort .............................O(n log n)
//   a fixed number of steps ......... O(1)
//
// This file COMPILES AND RUNS -- it just prints each function's answer so you
// can call it.  Cover the SOLUTIONS.md; reason from the code, not the name.

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

// (1)
int first_element(const std::vector<int>& v) {
  return v.empty() ? -1 : v[0];
}

// (2)
std::int64_t sum_all(const std::vector<int>& v) {
  std::int64_t total = 0;
  for (int x : v) {
    total += x;
  }
  return total;
}

// (3)
bool any_pair_sums_to(const std::vector<int>& v, int target) {
  for (std::size_t i = 0; i < v.size(); ++i) {
    for (std::size_t j = i + 1; j < v.size(); ++j) {
      if (v[i] + v[j] == target)
        return true;
    }
  }
  return false;
}

// (4) -- v is assumed ALREADY SORTED
bool contains_sorted(const std::vector<int>& v, int target) {
  return std::binary_search(v.begin(), v.end(), target);
}

// (5)
int count_digits(int n) {
  int digits = 0;
  while (n != 0) {
    n /= 10;  // strips one digit each step
    ++digits;
  }
  return digits;
}

int main() {
  std::vector<int> v{5, 2, 9, 1, 7, 3};
  std::vector<int> sorted_v = v;
  std::sort(sorted_v.begin(), sorted_v.end());

  std::cout << "first_element     -> " << first_element(v) << "\n";
  std::cout << "sum_all           -> " << sum_all(v) << "\n";
  std::cout << "any_pair_sums_to  -> " << any_pair_sums_to(v, 10) << "\n";
  std::cout << "contains_sorted   -> " << contains_sorted(sorted_v, 7) << "\n";
  std::cout << "count_digits(40320) -> " << count_digits(40320) << "\n";
  std::cout << "\nNow check your five answers against SOLUTIONS.md.\n";
}
