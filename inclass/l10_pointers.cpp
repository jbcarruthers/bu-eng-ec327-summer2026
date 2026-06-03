// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: the core L10 picture -- a reference is an alias; a pointer is a
//          variable whose value is an address. `&` and `*` are inverses.
// Topic: pointers
// Category: canonical
// Lecture: L10
// Build: g++ -std=c++20 -O0 -g l10_pointers.cpp -o l10_pointers

// Cashing in yesterday's exit ticket: in GDB you saw `&v` print an address.
// A POINTER is just a C++ variable that holds one -- the machine's "register
// holding a number that is an address" (L8's `[rbx]`), now with a type and
// syntax. A REFERENCE is the safe 90% tool you already met in L4 (`const&`):
// a second name for an object that already exists -- no null, no reseating.
//
//   x        an int                       (lives on the stack)
//   &x       the address OF x             (address-of)
//   int* p   a pointer variable           (holds an address)
//   p = &x   point p at x
//   *p       the int p points at          (dereference -- inverse of &)
//   int& r   a reference                  (another name for x; not a copy)
//
// GDB walk (see lecture_10_slides.tex, "box and arrow" frame):
//   break 40 ; run
//   p &x        ; an address, e.g. 0x16f...c
//   p p         ; the SAME address -- p holds it
//   p *p        ; 42 -- follow the arrow
//   p &r        ; identical to &x -- a reference is not its own object
#include <iostream>

int main() {
  int x = 42;       // a plain int on the stack
  int* p = &x;      // p HOLDS the address of x
  int& r = x;       // r is another NAME for x (an alias) -- not a copy

  std::cout << "x  = " << x << "        (the value)\n";
  std::cout << "&x = " << &x << "  (x's address)\n";
  std::cout << "p  = " << p << "  (p holds that same address)\n";
  std::cout << "*p = " << *p << "        (follow the arrow back to x)\n\n";

  *p = 99;          // write THROUGH the pointer...
  std::cout << "after *p = 99:  x = " << x << "   (changed x itself)\n";

  r = 7;            // ...write through the reference -- same effect, no `*`
  std::cout << "after  r = 7:   x = " << x << "   (r IS x)\n\n";

  // A pointer can be re-seated; a reference cannot.
  int y = 1000;
  p = &y;           // legal: p now points at y
  // int& r2; r2 = y;  // ILLEGAL: a reference must bind at birth, forever
  std::cout << "p re-seated to y: *p = " << *p << "\n";

  // nullptr: a pointer that points at NOTHING. References can't be null.
  int* nothing = nullptr;
  std::cout << "nothing = " << nothing << " (points nowhere -- do NOT *it)\n";
  // std::cout << *nothing;   // <-- SEGFAULT (Hoare's billion-dollar mistake)
  return 0;
}
