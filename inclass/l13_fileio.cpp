// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: Reading and writing files with streams -- ofstream, ifstream,
//          getline, >>, the failure check, and RAII close.
// Topic: io-files
// Category: demo
// Lecture: L13

// L13, part 3. A file stream is the SAME << / >> you already use on
// std::cout / std::cin -- just pointed at a file instead of the terminal.
// That is the whole idea of "streams": one interface, many endpoints.
//
//   std::ofstream  -- OUTPUT file stream; you << into it (writes the file).
//   std::ifstream  -- INPUT  file stream; you >> or getline out of it (reads).
//
// Two things that bite Python programmers moving to C++:
//   1. OPENING CAN FAIL and C++ will not throw -- it sets a flag. You must
//      CHECK (if (!in) ... or in.is_open()). A file that does not exist, or a
//      directory you cannot write, fails silently otherwise.
//   2. You do NOT call close(). The stream is an RAII object (L6): its
//      destructor closes the file when it goes out of scope. Same guarantee
//      as unique_ptr (L11) -- the resource is tied to the object's lifetime.
//
// Build:  g++ -std=c++20 -Wall -Wextra -O0 -g l13_fileio.cpp -o l13_fileio
// Run:    ./l13_fileio          (writes ./scores.txt, then reads it back)

#include <format>
#include <fstream>  // std::ofstream, std::ifstream
#include <iostream>
#include <string>

namespace {

const std::string kPath = "scores.txt";

// WRITE a small data file: one "name score" per line.
// Return false if the file could not be opened for writing.
bool write_scores() {
  std::ofstream out(kPath);  // opens (truncates) the file
  if (not out) {             // ALWAYS check -- opening can fail silently
    std::cerr << std::format("could not open {} for writing\n", kPath);
    return false;
  }
  // << works exactly like it does on std::cout, and std::format composes:
  out << std::format("{} {}\n", "Ada", 92);
  out << std::format("{} {}\n", "Alan", 88);
  out << std::format("{} {}\n", "Grace", 95);
  std::cout << std::format("wrote 3 rows to {}\n", kPath);
  return true;
  // No out.close() -- the ofstream destructor flushes and closes here (RAII).
}

// READ it back two ways, to show the two reading idioms.
bool read_scores() {
  std::ifstream in(kPath);
  if (not in) {
    std::cerr << std::format("could not open {} for reading\n", kPath);
    return false;
  }

  // (a) Line at a time with std::getline -- the right tool when a "record"
  //     is a whole line (and may contain spaces).
  std::cout << "\nline-by-line (std::getline):\n";
  std::string line;
  while (std::getline(in, line)) {  // loop condition IS the read+check
    std::cout << std::format("  | {}\n", line);
  }

  // Rewind to the top and read again, this time field-by-field.
  in.clear();   // clear the end-of-file flag from the last loop
  in.seekg(0);  // back to byte 0

  // (b) Field at a time with >> -- skips whitespace, converts as it goes.
  //     The loop ends when a read FAILS (eof, or a non-number where a number
  //     was expected). This is the idiomatic "read until the stream is dry."
  std::cout << "\nfield-by-field (>> with conversion):\n";
  std::string name;
  int score = 0;
  int total = 0;
  int count = 0;
  while (in >> name >> score) {  // reads a word, then an int; both must succeed
    std::cout << std::format("  {:<6} {:>3}\n", name, score);
    total += score;
    ++count;
  }
  if (count > 0) {
    std::cout << std::format("  {:<6} {:>6.1f}  (mean)\n", "avg",
                             static_cast<double>(total) / count);
  }
  return true;
}

}  // namespace

int main() {
  if (not write_scores()) {
    return 1;
  }
  if (not read_scores()) {
    return 1;
  }
  return 0;
}
