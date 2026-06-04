// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: move semantics conceptually -- cheap transfer vs expensive copy;
//          std::move is a cast that says "you may pillage this"; never touch a
//          moved-from object expecting its old value. Pays off L9's vector
//          cost model (the 3-word handle vs the heap buffer).
// Topic: move-semantics
// Category: demo
// Lecture: L11
// Build: g++ -std=c++20 -O0 -g l11_move.cpp -o l11_move

// L9 drew a std::vector as a small 3-word HANDLE on the stack (data ptr, size,
// capacity) pointing at a buffer on the HEAP. That picture explains the whole
// idea of moving:
//   * COPY a vector  -> allocate a NEW heap buffer and copy every element.
//                       O(n). Expensive for a big vector.
//   * MOVE a vector  -> just hand over the 3-word handle and null out the old
//                       one. O(1). The heap buffer never moves -- only the
//                       handle's owner changes.
// std::move does not move anything by itself: it is a CAST that marks an object
// as "fair game to pillage," letting the move constructor run instead of the
// copy constructor. After a move, the source is in a VALID but UNSPECIFIED
// state -- for std::vector that means empty. Don't read its old value.
#include <iostream>
#include <utility>  // std::move
#include <vector>

int main() {
  std::vector<int> big(5, 7);   // 5 elements, all 7 -- a small stand-in
  std::cout << "start: big.size() = " << big.size() << '\n';

  std::cout << "\n1) COPY -- both vectors own independent heap buffers:\n";
  std::vector<int> copy = big;  // deep copy: new buffer, all elements copied
  std::cout << "  big.size()  = " << big.size()
            << "   copy.size() = " << copy.size()
            << "   (big still intact)\n";

  std::cout << "\n2) MOVE -- hand over the handle; the buffer never moves:\n";
  std::vector<int> moved = std::move(big);  // big's handle transfers to moved
  // Reading the moved-from `big` is exactly what this demo is about: it is now
  // EMPTY but still a valid object. clang-tidy's use-after-move warns here --
  // deliberately suppressed, because observing the moved-from state IS the
  // lesson. (In real code, reading a moved-from value is the bug it flags.)
  std::cout << "  moved.size() = " << moved.size() << '\n';
  std::cout << "  big.size()   = "
            << big.size()  // NOLINT(bugprone-use-after-move) -- the point
            << "   (big is now EMPTY -- moved-from)\n";

  std::cout << "\n3) moved-from is valid -- you may REASSIGN, just don't assume"
               " its old value:\n";
  big = {1, 2, 3};              // perfectly fine: a fresh value
  std::cout << "  big reassigned: big.size() = " << big.size() << '\n';

  std::cout << "\n4) why it matters -- returning a big object is a MOVE, free:\n";
  auto make_data = [] {
    std::vector<int> local(1000, 0);
    return local;               // returned by move (or elided) -- NOT copied
  };
  std::vector<int> data = make_data();
  std::cout << "  got " << data.size() << " elements with no copy\n";

  std::cout << "\n(unique_ptr is move-ONLY for the same reason: copying it would"
               " mean two owners. Move = transfer of ownership.)\n";
  return 0;
}
