// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: SAME heap int as l11_raw_int.cpp, but the handle now frees it
//          automatically. sizeof(unique_ptr<int>) == sizeof(int*): the safety
//          is FREE -- no extra bytes, no extra allocation. RAII, zero overhead.
// Topic: modern-pointers
// Category: canonical
// Lecture: L11
// Build: g++ -std=c++20 -O0 -g l11_unique_int.cpp -o l11_unique_int

// Compare to l11_raw_int.cpp. The memory picture is IDENTICAL:
//
//   STACK                         HEAP
//   +-----------+                 +--------+
//   |  u  ------|---------------->|   42   |   <- make_unique<int>(42)
//   +-----------+                 +--------+
//   a unique_ptr<int>             the int -- ONE allocation, same as `new int`
//
// What changed is NOT the layout -- it's WHO frees the heap int. `u` is a stack
// object, so its destructor runs at the closing brace, and that destructor
// calls delete for you. You cannot forget; you cannot do it twice. The leak,
// dangling, and double-free of the raw version are now unwriteable.
//
// And it costs nothing: a unique_ptr<int> is the SAME 8 bytes as a raw int*.
// It IS a raw pointer -- just one that remembers to free itself.
#include <iostream>
#include <memory>

int main() {
  std::cout << "unique_ptr<int>  (the handle frees the heap int for you)\n";
  {
    auto u = std::make_unique<int>(42);  // one heap int, owned by `u`

    std::cout << "  &u        = " << &u
              << "   <- the unique_ptr OBJECT (on the STACK)\n";
    std::cout << "  u.get()   = " << u.get()
              << "   <- the heap address it owns (same kind as raw `p`)\n";
    std::cout << "  *u        = " << *u << "   <- deref just like a pointer\n";
    std::cout << "  sizeof u  = " << sizeof(u)
              << " bytes   <- IDENTICAL to a raw int*. zero overhead.\n";

    std::cout << "  ...leaving the scope now; watch -- no delete is written:\n";
  }  // <-- `u` dies HERE. Its destructor calls delete. The heap int is freed.
  std::cout << "  back in main: the heap int was freed at the brace. No leak,\n"
               "  no dangling, no double-free -- the type guaranteed it.\n";
  return 0;
}
