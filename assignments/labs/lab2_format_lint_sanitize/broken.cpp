// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: Lab 2 sanitizer target — compiles clean, runs, is quietly on fire.
// Topic: sanitizers
// Category: shell
// Lecture: lab2
// Build: g++ -std=c++20 -g -O0 -fsanitize=address,undefined broken.cpp -o broken
//
// This program compiles with zero warnings (try it: -Wall -Wextra say nothing)
// and, built normally, appears to run fine. It has two bugs that the language
// calls "undefined behavior" — the compiler is allowed to do ANYTHING, which
// today happens to look like "works," and tomorrow looks like a crash, a wrong
// answer, or a security hole. Part 3 rebuilds this under ASan + UBSan so you
// can watch both bugs get caught the instant they happen.

#include <climits>
#include <iostream>

int main() {
  // Bug 1 (UBSan catches this): signed integer overflow. INT_MAX + 1 cannot be
  // represented in an int. UBSan prints a one-line runtime error and, by
  // default, keeps going.
  int big = INT_MAX;
  int wrapped = big + 1;
  std::cout << "wrapped = " << wrapped << '\n';

  // Bug 2 (ASan catches this): heap-buffer-overflow. We allocate 4 ints
  // (valid indices 0..3) but the loop condition `i <= 4` writes data[4], one
  // past the end. ASan stops the program cold and prints a full report.
  int* data = new int[4];
  for (int i = 0; i <= 4; ++i) {
    data[i] = i * i;
  }
  std::cout << "data[4] = " << data[4] << '\n';

  delete[] data;
  return 0;
}
