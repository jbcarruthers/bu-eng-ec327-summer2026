// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: The same output, three ways -- printf vs cout<< vs std::format.
// Topic: io-formatting
// Category: demo
// Lecture: L13

// L13, part 1. C++ gives you THREE dialects for turning values into text.
// They are not three styles of the same thing -- they make different
// promises. The point of this demo is to print ONE line of output three
// ways and watch what each dialect costs you and guarantees you.
//
//   printf       -- C heritage (1972, Ritchie). A format string with %-codes
//                   and a variadic argument list. Terse. Everywhere. But the
//                   compiler does NOT check that your % codes match your
//                   arguments -- that mismatch is undefined behavior.
//   std::cout << -- idiomatic C++ since 1985. Type-SAFE: << knows the type of
//                   each value, so you can't mismatch. But formatting (width,
//                   precision, base) means fiddly <iomanip> manipulators that
//                   are sticky and verbose.
//   std::format  -- C++20. Python-style "{}" placeholders. Type-safe AND the
//                   format string is checked at COMPILE time. The modern
//                   default. (std::print in C++23 fuses format + output.)
//
// Build:  g++ -std=c++20 -Wall -Wextra -O0 -g l13_dialects.cpp -o l13_dialects
// Run:    ./l13_dialects

#include <cstdio>    // printf
#include <format>    // std::format  (C++20)
#include <iomanip>   // std::setw, std::setprecision, std::fixed
#include <iostream>  // std::cout
#include <string>

namespace {

// We will print this same fact three ways:
//   Ada       scored  92.50% over   3 runs
// A left-justified name in a 8-wide field, a fixed 2-decimal percentage in a
// 6-wide field, and a right-justified integer in a 3-wide field.

void with_printf(const std::string& name, double pct, int runs) {
  // %-8s  : string, left-justified (the minus), 8 wide
  // %6.2f : double, 6 wide, 2 after the decimal point
  // %3d   : int, 3 wide, right-justified
  // The compiler trusts you. If a code and an argument disagree, that is
  // undefined behavior -- see the footgun() below.  c_str() because printf
  // is a C function that wants a const char*, not a std::string.
  std::printf("printf  : %-8s scored %6.2f%% over %3d runs\n", name.c_str(),
              pct, runs);
}

void with_cout(const std::string& name, double pct, int runs) {
  // Type-safe: << knows each type. But the formatting is manipulators, and
  // some of them are STICKY (std::setprecision / std::fixed stay set for the
  // rest of the stream); std::setw applies only to the very next item.
  std::cout << "cout    : " << std::left << std::setw(8) << name << " scored "
            << std::right << std::fixed << std::setprecision(2) << std::setw(6)
            << pct << "% over " << std::setw(3) << runs << " runs\n";
}

void with_format(const std::string& name, double pct, int runs) {
  // One readable string. {:<8} left, width 8; {:6.2f} width 6, 2 decimals;
  // {:>3} right, width 3. If a placeholder and an argument disagree, this
  // does NOT compile -- the format string is checked at compile time.
  std::cout << std::format("format  : {:<8} scored {:6.2f}% over {:>3} runs\n",
                           name, pct, runs);
  // C++23 sequel -- one call, no <<:  std::print("{:<8} ...\n", name, ...);
}

// The reason std::format is the default: TYPE SAFETY.  printf is variadic and
// unchecked.  The line below asks printf to read a double as if it were an
// int -- the format string and the argument disagree.  There is no compiler
// error; at runtime you get garbage (or a crash). g++'s -Wformat warns here
// (always compile with -Wall), but nothing in the LANGUAGE stops you.
//
//   std::printf("%d\n", 3.14);   // <- undefined behavior, prints garbage
//   std::format("{}\n", 3.14);   // <- prints 3.14, no way to mismatch
//
// We do not run the broken printf (it is UB); we show its safe twin instead.
void footgun_explained() {
  const double x = 3.14;
  std::cout << "\nWhy std::format is the default -- type safety:\n";
  std::cout << "  printf(\"%d\", 3.14)  -> UNDEFINED BEHAVIOR (no compiler "
               "error; -Wformat warns)\n";
  std::cout << std::format(
      "  format(\"{{}}\", 3.14) -> {}  (cannot mismatch; "
      "checked at compile time)\n",
      x);
}

}  // namespace

int main() {
  const std::string name = "Ada";
  const double pct = 92.5;
  const int runs = 3;

  std::cout << "Same line, three dialects:\n";
  with_printf(name, pct, runs);
  with_cout(name, pct, runs);
  with_format(name, pct, runs);

  footgun_explained();
  return 0;
}
