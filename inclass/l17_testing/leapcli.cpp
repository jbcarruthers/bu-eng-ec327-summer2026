// EC327 L17 -- leapcli.cpp
// A thin command-line wrapper around the SAME is_leap_year unit.
//   ./leapcli 2000      -> prints "2000 is a leap year",  exit code 0
//   ./leapcli 2023      -> prints "2023 is not a leap year", exit code 1
//   ./leapcli hello      -> prints a usage error to stderr,  exit code 2
//
// Why build this in addition to test_leap.cpp?  So we can test the program
// from the OUTSIDE -- as a black box -- using a different harness (pytest,
// driving the compiled binary via subprocess; see test_leapcli.py).  Same
// logic, two angles: white-box unit tests in C++, black-box CLI tests in
// Python.  The exit code is part of the contract a CLI test checks.
//
// Build: g++ -std=c++20 -Wall -Wextra leapcli.cpp -o leapcli

#include <charconv>
#include <iostream>
#include <string>

#include "leap.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: leapcli <year>\n";
    return 2;
  }

  const std::string arg = argv[1];
  int year = 0;
  auto [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), year);
  if (ec != std::errc{} or ptr != arg.data() + arg.size()) {
    std::cerr << "error: '" << arg << "' is not an integer year\n";
    return 2;
  }

  if (is_leap_year(year)) {
    std::cout << year << " is a leap year\n";
    return 0;
  }
  std::cout << year << " is not a leap year\n";
  return 1;
}
