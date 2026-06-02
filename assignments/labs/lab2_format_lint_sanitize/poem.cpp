// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: Lab 2 two-file driver — calls greeting() from a separate TU.
// Topic: anatomy
// Category: canonical
// Lecture: lab2
// Origin: assignments/labs/lab1_unix_first_build/poem.cpp

#include <iostream>

#include "greet.h"

int main() {
  std::cout << "Every program begins\n";
  std::cout << "the same small way:\n";
  std::cout << greeting("EC327") << '\n';
  return 0;
}
