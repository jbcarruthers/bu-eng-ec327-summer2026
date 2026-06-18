// EC327 L16 demo -- "measure, don't guess": the timing race.
// Build: g++ -std=c++20 -O2 -Wall -Wextra race.cpp -o race && ./race
//
// TWO ways to answer "does this vector contain a duplicate?":
//   has_dup_quadratic : nested loop over every pair          -> O(n^2)
//   has_dup_sorted    : sort a copy, scan adjacent pairs     -> O(n log n)
// Both return the SAME answer.  We time them on the SAME data at growing n
// and print a table.  Watch the O(n^2) column explode while the O(n log n)
// column barely moves -- that gap is the whole point of big-O made visible.
//
// Note we build with -O2: timing a -O0 build measures the wrong program.

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

// O(n^2): compare every pair.  Innocent-looking, fatal at scale.
bool has_dup_quadratic(const std::vector<int>& v) {
  for (std::size_t i = 0; i < v.size(); ++i) {
    for (std::size_t j = i + 1; j < v.size(); ++j) {
      if (v[i] == v[j])
        return true;
    }
  }
  return false;
}

// O(n log n): sort a copy, then a single adjacent scan.
bool has_dup_sorted(std::vector<int> v) {  // by value -- we sort our own copy
  std::sort(v.begin(), v.end());
  for (std::size_t i = 1; i < v.size(); ++i) {
    if (v[i] == v[i - 1])
      return true;
  }
  return false;
}

// Distinct values (no duplicate), so BOTH algorithms hit their worst case:
// the quadratic one never short-circuits, the sorted one scans the whole copy.
std::vector<int> make_data(std::size_t n) {
  std::vector<int> v(n);
  for (std::size_t i = 0; i < n; ++i) {
    v[i] = static_cast<int>(i);
  }
  return v;
}

template <typename F>
double time_ms(F f) {
  auto t0 = std::chrono::steady_clock::now();
  volatile bool sink = f();  // volatile so the optimizer can't delete the call
  (void)sink;
  auto t1 = std::chrono::steady_clock::now();
  return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

int main() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "        n |   O(n^2) ms | O(n log n) ms |  ratio\n";
  std::cout << "----------+-------------+---------------+--------\n";
  for (std::size_t n : {1000u, 2000u, 4000u, 8000u, 16000u, 32000u}) {
    auto v = make_data(n);
    double q = time_ms([&] { return has_dup_quadratic(v); });
    double s = time_ms([&] { return has_dup_sorted(v); });
    std::cout << std::setw(9) << n << " | " << std::setw(11) << q << " | "
              << std::setw(13) << s << " | " << std::setw(5)
              << (s > 0 ? q / s : 0.0) << "x\n";
  }
  std::cout << "\nDouble n -> O(n^2) ~4x the time; O(n log n) ~2x.  "
               "That divergence is big-O, on the clock.\n";
}
