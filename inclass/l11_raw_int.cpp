// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: WHERE the bytes live -- a raw `int*` is a 1-word HANDLE on the stack
//          pointing at an int the heap handed you. You allocate it; you must
//          free it. The baseline the smart-pointer demos measure against.
// Topic: modern-pointers
// Category: demo
// Lecture: L11
// Build: g++ -std=c++20 -O0 -g l11_raw_int.cpp -o l11_raw_int

// L10 introduced new/delete. This demo zooms in on the MEMORY PICTURE so the
// next two (unique_ptr, shared_ptr) have something to compare against. The
// single int 42 lives in three places across these three files; only the
// bookkeeping around it changes.
//
//   STACK                         HEAP
//   +-----------+                 +--------+
//   |  p  ------|---------------->|   42   |   <- new int(42)
//   +-----------+                 +--------+
//   one machine word (8 bytes)    the int you asked for
//
// `p` (the pointer variable) is a local -> it lives on the stack and dies at
// the closing brace automatically. The `42` it points at lives on the heap and
// does NOT die on its own -- YOU say when, with `delete`. That asymmetry is the
// whole problem L11 exists to solve.
#include <iostream>

int main() {
  int* p = new int(42);  // ask the heap for one int; p HOLDS its address

  std::cout << "raw int*  (you manage the lifetime by hand)\n";
  std::cout << "  &p        = " << &p
            << "   <- the pointer VARIABLE itself (on the STACK)\n";
  std::cout << "  p         = " << p
            << "   <- the address it holds (on the HEAP)\n";
  std::cout << "  *p        = " << *p << "   <- follow the arrow: the int\n";
  std::cout << "  sizeof p  = " << sizeof(p)
            << " bytes   <- a pointer is one machine word, whatever it points at\n";
  // Note the two addresses live in different REGIONS: &p (the stack) and p (the
  // heap) sit far apart in memory -- that split is the whole point.

  delete p;     // give the heap int back -- exactly once, at the right time
  p = nullptr;  // the handle still exists (it's a stack variable); empty it

  std::cout << "  after delete: the heap int is gone; the handle p still exists,\n"
               "  now nullptr. WHO was responsible? You were. Nothing checked.\n";
  return 0;
}
