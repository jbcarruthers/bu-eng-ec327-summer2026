// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: why std::vector exists -- the raw array's three sins (decay, no
//          size, no bounds check) and that `a[i]` is just `*(a + i)`.
// Topic: pointers
// Category: demo
// Lecture: L10
// Build: g++ -std=c++20 -O0 -g l10_array_decay.cpp -o l10_array_decay

// A raw array is a block of elements with NO handle around it. Three sins:
//   1. it DECAYS to a pointer the moment you pass it -- the size is lost
//   2. it doesn't know its own length (sizeof works only in its home scope)
//   3. `a[i]` does no bounds checking -- it is literally `*(a + i)`
// std::vector fixes all three: it carries its size, never decays, and `.at()`
// checks. THIS is the "why" behind the L9 vector picture.
//
// Build-to-learn / use-to-ship: you write a raw array to SEE the mechanism;
// you ship std::vector because it is the same buffer, made safe.
#include <iostream>
#include <vector>

// The array DECAYS to `int*` here -- `arr` is a pointer, not an array.
void sins(int arr[]) {
  // sizeof(arr) is sizeof(int*) == 8, NOT the array's bytes. The size is gone.
  std::cout << "  inside sins(): sizeof(arr) = " << sizeof(arr)
            << " bytes  (a pointer, not the array!)\n";
}

int main() {
  int a[5] = {10, 20, 30, 40, 50};

  // In its home scope, sizeof knows the whole array.
  std::cout << "sizeof(a) = " << sizeof(a) << " bytes  ("
            << sizeof(a) / sizeof(a[0]) << " ints)\n";
  sins(a);  // ...but the moment we pass it, the length is lost.

  // a[i] IS *(a + i): subscript is pointer arithmetic in disguise.
  std::cout << "a[2]      = " << a[2] << "\n";
  std::cout << "*(a + 2)  = " << *(a + 2) << "   (identical -- [] is sugar)\n";
  std::cout << "a == &a[0]? " << (a == &a[0] ? "yes" : "no")
            << "   (the array name decays to a pointer to its first element)\n";

  // Sin #3: no bounds check. a[7] reads two ints PAST the end -- garbage or a
  // crash, no warning. This is the buffer-overrun class of security bugs.
  //   std::cout << a[7];   // <-- UNDEFINED BEHAVIOR (out of bounds)

  // The ship answer: std::vector carries its size and offers a checked .at().
  std::vector<int> v{10, 20, 30, 40, 50};
  std::cout << "\nv.size()  = " << v.size() << "   (the vector KNOWS)\n";
  try {
    v.at(7);  // checked: throws instead of corrupting memory
  } catch (const std::out_of_range& e) {
    std::cout << "v.at(7)   threw std::out_of_range  (caught, not corrupted)\n";
  }
  return 0;
}
