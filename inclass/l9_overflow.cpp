// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: a stack overflow, on purpose — runaway recursion with no base case.
// Topic: memory model
// Category: demo
// Lecture: L9
// Build: g++ -std=c++20 -O0 -g l9_overflow.cpp -o l9_overflow
// WARNING: this program crashes by design (segfault / stack exhaustion).

// The failure mode that gives "Stack Overflow" its name. `descend` calls
// itself forever; every call pushes a frame; the stack grows downward until
// it runs out of room and the OS kills the program. The `depth` counter +
// the local `pad` array make the growth visible and fast.
//
// Run it (it WILL crash — that is the demo):
//   g++ -std=c++20 -O0 -g l9_overflow.cpp -o l9_overflow
//   ./l9_overflow            # prints rising depths, then "Segmentation fault"
//
// In GDB, catch the crash and look at how deep it got:
//   gdb ./l9_overflow
//     run                    # let it crash
//     bt                     # thousands of identical frames — the smoking gun
//     # (use `bt 5` — a full backtrace of a blown stack is enormous)
//
// THE FIX is one line: a base case. Add `if (depth > 5) return;` at the top
// and the recursion terminates — exactly the `n <= 1` guard in l8_factorial.
#include <iostream>

void descend(long depth) {
  volatile char pad[1];  // burn stack faster; volatile => not optimized away
  pad[0] = 0;
  // if (depth % 1000 == 0) {
  std::cout << "depth = " << depth << " " << &depth << " " << sizeof(depth)
            << '\n';
  // }
  descend(depth + 1);  // no base case — the bug
}

int main() {
  std::cout << "descending with no base case (this will crash)...\n";
  descend(0);
  std::cout << "unreachable\n";
  return 0;
}
