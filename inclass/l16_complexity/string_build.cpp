// EC327 L16 demo -- the quadratic-string footgun ("Schlemiel the Painter").
// Build: g++ -std=c++20 -O2 -Wall -Wextra string_build.cpp -o sb && ./sb
//
// Building one big string from many pieces, two ways:
//   slow : out = out + piece;   re-copies ALL of out every pass  -> O(n^2)
//   fast : out += piece;        appends in place                 -> O(n)
// Same result; the only change is `+` vs `+=`.  Time both and watch the slow
// one fall off a cliff as the number of pieces grows.  This is the exact
// footgun from the opener -- accidental O(n^2) hiding in one innocent line.

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

std::string build_slow(const std::vector<std::string>& pieces) {
  std::string out;
  for (const auto& p : pieces) {
    out = out + p;  // O(n^2): re-copies all of out each time
  }
  return out;
}

std::string build_fast(const std::vector<std::string>& pieces) {
  std::string out;
  for (const auto& p : pieces) {
    out += p;  // O(n): in-place append
  }
  return out;
}

template <typename F>
double time_ms(F f) {
  auto t0 = std::chrono::steady_clock::now();
  std::string s = f();
  auto t1 = std::chrono::steady_clock::now();
  volatile std::size_t sink = s.size();  // keep the result alive
  (void)sink;
  return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

int main() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "   pieces |  out = out + p (ms) |  out += p (ms) |  ratio\n";
  std::cout << "----------+---------------------+----------------+--------\n";
  for (std::size_t n : {2000u, 4000u, 8000u, 16000u, 32000u}) {
    std::vector<std::string> pieces(n, "some moderately long chunk of text. ");
    double slow = time_ms([&] { return build_slow(pieces); });
    double fast = time_ms([&] { return build_fast(pieces); });
    std::cout << std::setw(9) << n << " | " << std::setw(19) << slow << " | "
              << std::setw(14) << fast << " | " << std::setw(5)
              << (fast > 0 ? slow / fast : 0.0) << "x\n";
  }
  std::cout << "\nOne character -- `+` vs `+=` -- is the difference between "
               "O(n^2) and O(n).\n";
}
