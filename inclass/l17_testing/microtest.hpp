// EC327 L17 -- microtest.hpp
// A ~60-line header-only unit-test framework.  Build-to-learn version of
// doctest / Catch2: the real ones are thousands of lines, but the IDEA fits on
// one screen.  A test framework is just (1) a way to register named test
// cases, (2) macros that check a claim and record pass/fail, and (3) a runner
// that calls them all and prints a summary.  No magic.
//
// Usage (see test_leap.cpp):
//   #include "microtest.hpp"
//   TEST_CASE("2000 is a leap year") { CHECK(is_leap_year(2000)); }
//   MICROTEST_MAIN          // expands to int main() that runs every TEST_CASE

#ifndef MICROTEST_HPP
#define MICROTEST_HPP

#include <cstdio>
#include <functional>
#include <string>
#include <vector>

namespace microtest {

struct Case {
  std::string name;
  std::function<void()> fn;
};

// One global registry of test cases.  A function-local static so it is
// constructed on first use, no static-init-order surprises across files.
inline std::vector<Case>& registry() {
  static std::vector<Case> cases;
  return cases;
}

// Per-run counters for the summary line.
inline int& checks_run() {
  static int n = 0;
  return n;
}
inline int& checks_failed() {
  static int n = 0;
  return n;
}

// A tiny helper object whose constructor pushes a case into the registry.
// The TEST_CASE macro defines one static instance of this, so the
// registration happens before main() runs.
struct Registrar {
  Registrar(const std::string& name, std::function<void()> fn) {
    registry().push_back({name, std::move(fn)});
  }
};

inline void record(bool ok, const char* expr, const char* file, int line) {
  ++checks_run();
  if (!ok) {
    ++checks_failed();
    std::printf("  FAIL  %s:%d   CHECK(%s)\n", file, line, expr);
  }
}

inline int run_all() {
  int failed_cases = 0;
  for (const auto& c : registry()) {
    int before = checks_failed();
    std::printf("RUN   %s\n", c.name.c_str());
    c.fn();
    if (checks_failed() > before)
      ++failed_cases;
  }
  std::printf("\n%d checks, %d failed  (%zu test cases, %d failed)\n",
              checks_run(), checks_failed(), registry().size(), failed_cases);
  return checks_failed() == 0 ? 0 : 1;  // nonzero exit => CI sees red
}

}  // namespace microtest

// --- the macros students actually type -----------------------------------
// TEST_CASE("name") { ... }  -- glue a unique name to a lambda body and
// register it.  __COUNTER__ keeps the generated identifiers unique per file.
#define MICROTEST_CAT2(a, b) a##b
#define MICROTEST_CAT(a, b) MICROTEST_CAT2(a, b)
#define TEST_CASE(name)                                                \
  static void MICROTEST_CAT(microtest_fn_, __LINE__)();                \
  static microtest::Registrar MICROTEST_CAT(microtest_reg_, __LINE__)( \
      name, &MICROTEST_CAT(microtest_fn_, __LINE__));                  \
  static void MICROTEST_CAT(microtest_fn_, __LINE__)()

#define CHECK(expr) microtest::record((expr), #expr, __FILE__, __LINE__)
#define CHECK_EQ(a, b) CHECK((a) == (b))

#define MICROTEST_MAIN           \
  int main() {                   \
    return microtest::run_all(); \
  }

#endif  // MICROTEST_HPP
