// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: the smallest C++ -> assembly walk — one `add`, seen at every level.
// Topic: assembly
// Category: canonical
// Lecture: L8
// Build: g++ -arch x86_64 -std=c++20 -O0 -S -masm=intel l8_sum.cpp -o l8_sum.s

// Compare the assembly to the `sum` program in the L8 simulator.
// On Apple Silicon, add `-arch x86_64` to cross-emit x86 (else you get ARM64):
//   g++ -arch x86_64 -O0 -S -masm=intel l8_sum.cpp -o l8_sum_O0.s  # literal
//   g++ -arch x86_64 -O2 -S -masm=intel l8_sum.cpp -o l8_sum_O2.s  # optimized
//   g++          -O0 -S            l8_sum.cpp -o l8_sum_arm.s       # native ARM64
// On an Intel box / WSL2, drop -arch x86_64 (it's already x86).
int sum(int a, int b) {
  return a + b;
}

int main() {
  return sum(5, 7);  // exit status is the result; check with: echo $?
}
