// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: the lifetime problem, stated clearly -- new/delete shown ONCE, then
//          the three failure modes raw pointers can't prevent. The hook for L11.
// Topic: pointers
// Category: demo
// Lecture: L10
// Build:        g++ -std=c++20 -O0 -g l10_lifetime.cpp -o l10_lifetime
// Build (ASan): g++ -std=c++20 -O0 -g -fsanitize=address l10_lifetime.cpp -o l10_lifetime_asan

// new/delete is what std::vector does for you, underneath. You ask the heap
// for memory; you must give it back -- exactly once, at the right time. Raw
// pointers carry NO ownership information, so three bugs become possible:
//
//   LEAK        you never delete            -> memory grows forever
//   DANGLING    you delete, then use        -> use-after-free (UB)
//   DOUBLE-FREE you delete twice            -> heap corruption (UB)
//
// The unanswerable question raw pointers leave you with: given an `int* p`,
// WHO owns it and WHEN is it freed? You cannot tell by looking. L11's
// std::unique_ptr answers it by making ownership part of the type.
//
// Run the safe path (default). To SEE a use-after-free fire, build with ASan
// and uncomment the DANGLING block:
//   g++ -std=c++20 -O0 -g -fsanitize=address l10_lifetime.cpp && ./a.out
#include <iostream>

int main() {
  // --- new / delete, done correctly (shown once) ---------------------------
  int* p = new int(42);  // ask the heap for one int, value 42
  std::cout << "heap int @ " << p << " holds " << *p << '\n';
  delete p;              // give it back -- exactly once
  p = nullptr;           // defensive: a deleted pointer should point nowhere

  // --- FAILURE MODE 1: leak -------------------------------------------------
  // int* leaked = new int[1'000'000];   // ...and never delete[] it.
  // (void)leaked;                        // ASan/LeakSanitizer reports it at exit.

  // --- FAILURE MODE 2: dangling (use-after-free) ---------------------------
  // int* d = new int(7);
  // delete d;                            // freed...
  // std::cout << *d;                     // ...but read anyway. UB. ASan fires.

  // --- FAILURE MODE 3: double-free -----------------------------------------
  // int* e = new int(7);
  // delete e;
  // delete e;                            // freed twice. heap corruption. UB.

  std::cout << "safe path done. uncomment a block + build with ASan to see UB.\n";
  std::cout << "the open question: who owns a raw int*, and when is it freed?\n";
  std::cout << "  -> L11 answers it: std::unique_ptr makes ownership a type.\n";
  return 0;
}
