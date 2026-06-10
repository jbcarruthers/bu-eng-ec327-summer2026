// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: The std::format mini-language -- width, fill, align, precision, base.
// Topic: io-formatting
// Category: demo
// Lecture: L13

// L13, part 2. std::format has a tiny grammar inside the braces. If you know
// Python f-strings, you already know it -- it is deliberately the same:
//
//     {[index]:[fill][align][sign][#][0][width][.precision][type]}
//      \____/  \__________________________________________________/
//      which     the FORMAT SPEC -- everything after the colon
//      argument
//
// You do not memorize this. You learn the four or five pieces you actually
// use (align, width, precision, base) and look up the rest. This demo is a
// runnable cheat-sheet: each line prints a value and shows the spec that made
// it.  Compare to Python:  f"{pct:6.2f}"  is  std::format("{:6.2f}", pct).
//
// Build:  g++ -std=c++20 -Wall -Wextra -O0 -g l13_format.cpp -o l13_format
// Run:    ./l13_format

#include <format>
#include <iostream>
#include <numbers>  // std::numbers::pi  (C++20)

namespace {

void alignment_and_width() {
  std::cout << "ALIGN + WIDTH (the | marks the field edges):\n";
  // Default: numbers right-justify, text left-justifies. Override with < ^ >.
  std::cout << std::format("  left   {:<10}|\n", "Ada");
  std::cout << std::format("  center {:^10}|\n", "Ada");
  std::cout << std::format("  right  {:>10}|\n", "Ada");
  // A fill character goes BEFORE the alignment: '.' then '>'.
  std::cout << std::format("  dots   {:.>10}|\n", "Ada");
  std::cout << std::format("  zeros  {:0>10}|\n", 42);  // 0000000042
}

void numbers_and_precision() {
  std::cout << "\nPRECISION (for floating point):\n";
  const double pi = std::numbers::pi;
  std::cout << std::format("  default   {}\n", pi);       // full precision
  std::cout << std::format("  2 places  {:.2f}\n", pi);   // 3.14
  std::cout << std::format("  width.prec{:8.3f}\n", pi);  // "   3.142"
  std::cout << std::format("  percent   {:.1f}%\n", 92.5);
  // Scientific notation and an explicit + sign. (Note: C++ has NO Python-style
  // "{:,}" thousands separator -- grouping is locale-driven, "{:L}" with an
  // imbued std::locale, which we skip here.)
  std::cout << std::format("  scientif  {:.2e}\n", 1234567.0);  // 1.23e+06
  std::cout << std::format("  signed    {:+d}\n", 42);          // +42
}

void bases_and_types() {
  std::cout << "\nINTEGER BASES (the type letter at the end):\n";
  const int n = 255;
  std::cout << std::format("  decimal   {:d}\n", n);     // 255
  std::cout << std::format("  hex       {:x}\n", n);     // ff
  std::cout << std::format("  hex+0x    {:#x}\n", n);    // 0xff   (# = prefix)
  std::cout << std::format("  binary    {:#b}\n", n);    // 0b11111111
  std::cout << std::format("  octal     {:#o}\n", n);    // 0377
  std::cout << std::format("  pad hex   {:#06x}\n", n);  // 0x00ff
}

void positional_and_named() {
  std::cout << "\nARGUMENT INDICES (reuse / reorder arguments):\n";
  // The number before the colon picks WHICH argument. Reuse one twice:
  std::cout << std::format("  {0} squared is {1}, and {0} cubed is {2}\n", 3, 9,
                           27);
  // A literal brace is doubled: {{ prints a single {.
  std::cout << std::format("  a literal brace: {{ and }}\n");
}

}  // namespace

int main() {
  std::cout << "std::format mini-language -- a runnable cheat sheet\n";
  std::cout << "(same grammar as Python f-strings)\n\n";
  alignment_and_width();
  numbers_and_precision();
  bases_and_types();
  positional_and_named();

  std::cout << "\nThe whole grammar:\n";
  std::cout << "  {[index]:[fill][align][sign][#][0][width][.prec][type]}\n";
  std::cout << "  learn align/width/precision/base; look the rest up.\n";
  return 0;
}
