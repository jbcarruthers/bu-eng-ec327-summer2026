// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: a branch at the machine level — `cmp` sets flags, a jump reads them.
// Topic: assembly
// Category: canonical
// Lecture: L8
// Build: g++ -arch x86_64 -std=c++20 -O0 -S -masm=intel l8_max.cpp -o l8_max.s

// On Apple Silicon, `-arch x86_64` cross-emits x86 (drop it on an Intel box).
// Compare to the `max` program in the L8 simulator: cmp + a conditional jump.
// At -O2 the compiler replaces the branch with a conditional move (cmov) —
// a great "what the optimizer does" moment.
int max2(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

int main() {
  return max2(3, 9);  // echo $?  -> 9
}
