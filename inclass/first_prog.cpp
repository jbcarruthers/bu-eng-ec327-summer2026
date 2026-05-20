// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: the first program — anatomy of a C++ file: includes, main, statements.
// Topic: anatomy
// Category: canonical
// Lecture: L1

#include <iostream>
#include <string>

int main() {
  std::string course = "EC327";
  std::cout << "Welcome to " << course << ".\n";
  return 0;
}
