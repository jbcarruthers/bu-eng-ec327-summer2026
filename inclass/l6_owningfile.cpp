// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L6 -- RAII demo. A class that owns a FILE* opens it in the
//          constructor and closes it in the destructor. The caller
//          never writes a close() call; the language runs the dtor
//          when the object goes out of scope. Print on both ends so
//          students *see* the lifecycle.
// Topic: RAII, destructor, resource ownership
// Category: demo
// Lecture: L6
//
// Build & run:
//   g++ -std=c++20 -Wall l6_owningfile.cpp -o l6_owningfile
//   ./l6_owningfile
//   cat /tmp/l6_demo.log /tmp/l6_inner.log
//
// Pedagogical note: this class is deliberately INCOMPLETE -- it has a
// destructor but no copy ctor, copy assignment, move ctor, or move
// assignment. Per the Rule of Five (see L14), writing one of those
// five usually means you need all of them. The L6 message is: in real
// code, you would either write all five, or (better, Rule of Zero)
// use std::ofstream, which is an OwningFile for output files with
// the destructor already written for us.

#include <cstdio>
#include <iostream>

class OwningFile {
 public:
  // Acquire the resource in the constructor.
  explicit OwningFile(const char* path)
      : path_{path}, f_{std::fopen(path, "w")} {
    std::cout << "[ctor] opened " << path_ << "\n";
  }

  // Release the resource in the destructor. Runs automatically when
  // the object goes out of scope -- you do not call this yourself.
  ~OwningFile() {
    if (f_) {
      std::fclose(f_);
      std::cout << "[dtor] closed " << path_ << "\n";
    }
  }

  void write_line(const char* s) {
    if (f_) {
      std::fputs(s, f_);
      std::fputc('\n', f_);
    }
  }

 private:
  const char* path_;
  std::FILE*  f_;
};

int main() {
  std::cout << "--- entering main ---\n";

  OwningFile log{"/tmp/l6_demo.log"};
  log.write_line("hello classes II");

  // Open a nested scope to show dtor order: objects are destroyed in
  // REVERSE construction order as the scope unwinds.
  {
    std::cout << "--- entering inner block ---\n";
    OwningFile inner{"/tmp/l6_inner.log"};
    inner.write_line("written from the inner scope");
    std::cout << "--- leaving inner block ---\n";
    // inner's destructor runs here, before the closing brace returns
    // control to main.
  }

  log.write_line("inner is already closed; we are not");
  std::cout << "--- leaving main ---\n";
  return 0;
  // log's destructor runs as main's scope unwinds.
}
