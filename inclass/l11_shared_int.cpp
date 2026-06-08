// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: SAME heap int again -- but now ownership is SHARED. shared_ptr is
//          TWO words (int ptr + control-block ptr) and carries a reference
//          count; the int is freed when the LAST owner dies. The extra
//          allocation/word is what you pay for sharing -- so don't, unless you
//          need it.
// Topic: modern-pointers
// Category: demo
// Lecture: L11
// Build: g++ -std=c++20 -O0 -g l11_shared_int.cpp -o l11_shared_int

// Third time, same int 42 -- but the memory picture grows a second box:
//
//   STACK                  HEAP (one block from make_shared)
//   +-----------+          +-----------------------------+
//   |  s        |          |  control block: count = 1   |
//   |  .ptr  ---|--------->|  the int:       42          |
//   |  .ctrl ---|---------/                              |
//   +-----------+          +-----------------------------+
//   TWO words              make_shared puts the int AND the count side by side
//                          in a SINGLE allocation (shared_ptr(new int) makes two)
//
// The control block holds the reference count: how many shared_ptrs point here.
// Copy a shared_ptr -> count goes up. Destroy one -> count goes down. At zero,
// the int is freed. That bookkeeping is why a shared_ptr is bigger than a raw
// pointer and why make_shared does a little more work than `new`. Default to
// unique_ptr; reach for shared_ptr only when ownership is genuinely shared.
#include <iostream>
#include <memory>

int main() {
  std::cout << "shared_ptr<int>  (reference-counted shared ownership)\n";
  auto s = std::make_shared<int>(42);  // one heap block: the int + a count

  std::cout << "  &s         = " << &s
            << "   <- the shared_ptr OBJECT (on the STACK)\n";
  std::cout << "  s.get()    = " << s.get()
            << "   <- the heap int it points at\n";
  std::cout << "  *s         = " << *s << '\n';
  std::cout << "  sizeof s   = " << sizeof(s)
            << " bytes  <- TWO words: ptr-to-int + ptr-to-control-block\n";
  std::cout << "  use_count  = " << s.use_count() << "   <- just s, for now\n\n";

  std::cout << "  sharing bumps the count -- no new int is allocated:\n";
  {
    auto also = s;  // NOLINT(performance-unnecessary-copy-initialization)
                    // the copy IS the lesson: it bumps the ref count.
    std::cout << "    after `auto also = s;`  use_count = " << s.use_count()
              << "   (s and also point at the SAME int @ " << s.get() << ")\n";
  }  // `also` dies here -> count drops back
  std::cout << "    after `also` leaves scope  use_count = " << s.use_count()
            << "   (the int is NOT freed -- s still owns it)\n\n";

  std::cout << "  the int is freed only when the LAST shared_ptr dies\n"
               "  (here: when s leaves main). That's what the count buys you --\n"
               "  and why unique_ptr is the default until you PROVE you need this.\n";
  return 0;
}
