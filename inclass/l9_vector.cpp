// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: the vector picture, made real — the object on the stack, its
//          elements on the heap, seen in GDB.
// Topic: memory model
// Category: canonical
// Lecture: L9
// Build: g++ -std=c++20 -O0 -g l9_vector.cpp -o l9_vector

// The single most important picture of L9: a std::vector is a small object
// (3 words: a pointer, a size, a capacity) that lives wherever you declare it
// — here, on the stack — while the ELEMENTS it points at live on the HEAP,
// far away. The local int `tag` is the stack landmark to compare against.
//
// GDB walk (see lecture_09_slides.tex, "Live memory" frames):
//   g++ -std=c++20 -O0 -g l9_vector.cpp -o l9_vector
//   gdb ./l9_vector
//     break 28              # the line after v is filled
//     run
//     p &tag                # a stack address (near $rsp)
//     p &v                  # the vector OBJECT — also on the stack, near tag
//     p v.data()            # the ELEMENTS — a heap address, far from &v
//     p sizeof(v)           # 24 on a 64-bit build: 3 words, not 3 ints
//     x/3xw v.data()        # the three ints 10 20 30, sitting on the heap
//     p &v < (void*)v.data() ? "stack below heap" : "heap below stack"
//   Compare the magnitude of &v vs v.data(): same program, two regions.
#include <iostream>
#include <vector>

int main() {
  int tag = 42;                    // a plain local — lives on the stack
  std::vector<int> v{10, 20, 30};  // object on stack; elements on heap

  std::cout << "tag       @ " << &tag << '\n';
  std::cout << "v (object)@ " << &v << '\n';
  std::cout << "v.data()  @ " << v.data() << "   (the elements)\n";
  std::cout << "sizeof(v) = " << sizeof(v) << " bytes (3 words, not 3 ints)\n";
  return 0;
}
