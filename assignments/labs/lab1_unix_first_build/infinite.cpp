// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: counts upward forever — Lab 1 vehicle for Ctrl-C, jobs, kill.
// Topic: anatomy
// Category: canonical
// Lecture: lab1

#include <chrono>
#include <iostream>
#include <thread>

int main() {
  long n = 0;
  while (true) {
    std::cout << "tick " << ++n << '\n' << std::flush;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
