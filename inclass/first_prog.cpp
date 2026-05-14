// Copyright 2026 J. Carruthers <jbc@bu.edu>  ·  EC327 Summer 2026
// SPDX-License-Identifier: MIT
// Purpose: the first program — anatomy of a C++ file: includes, main, statements.

#include <iostream>
#include <string>

int main() {
  std::string course = "EC327";
  std::cout << "Welcome to " << course << ".\n";
  return 0;
}
