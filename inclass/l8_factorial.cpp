// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: recursion you can watch — the call stack growing in GDB `bt full`.
// Topic: assembly
// Category: canonical
// Lecture: L8,L9
// Build: g++ -std=c++20 -O0 -g l8_factorial.cpp -o l8_factorial

// The headliner for L8/L9. Two ways to look at it:
//   1) g++ -arch x86_64 -O0 -S -masm=intel l8_factorial.cpp -o l8_factorial.s
//      (on Apple Silicon; drop -arch x86_64 on an Intel box, or omit it to see
//       native ARM64) -> find `call _Z9factoriali` + the prologue/epilogue.
//   2) gdb ./l8_factorial   (built with -g)
//        break factorial
//        run
//        continue   (a few times)
//        bt full    -> SEE the stack frames stacked up, one per call
//      This is the real-x86 twin of the simulator's "factorial (recursive)".
#include <iostream>

int factorial(int n) {
  if (n <= 1) {
    return 1;       // base case
  }
  return n * factorial(n - 1);  // recursive case — pushes a new frame
}

int main() {
  std::cout << factorial(4) << '\n';  // 24
}
