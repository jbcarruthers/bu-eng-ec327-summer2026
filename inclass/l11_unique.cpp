// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: unique_ptr + make_unique as THE default owning pointer -- RAII frees
//          on scope exit; move-only ownership makes leak/dangling/double-free
//          impossible. The "use to ship" answer to L10's lifetime problem.
// Topic: modern-pointers
// Category: canonical
// Lecture: L11
// Build: g++ -std=c++20 -O0 -g l11_unique.cpp -o l11_unique

// L10 left one question hanging: given `int* p = new int(...)`, WHO owns it and
// WHEN is it freed? You couldn't tell by looking at the type. A unique_ptr puts
// the answer IN the type: it owns exactly one heap object and frees it
// automatically when it dies -- RAII (L6), now for any heap resource. `delete`
// never appears, and the three L10 failure modes become impossible:
//   * leak        -- the destructor always runs on scope exit
//   * dangling    -- you can't outlive the owner without moving ownership
//   * double-free -- there is only ever ONE owner (unique_ptr is move-only)
#include <iostream>
#include <memory>
#include <string>
#include <utility>  // std::move

// A noisy resource so we can SEE the destructor fire on scope exit.
struct Widget {
  std::string name;
  explicit Widget(std::string n) : name(std::move(n)) {
    std::cout << "  Widget(" << name << ") acquired\n";
  }
  ~Widget() { std::cout << "  ~Widget(" << name << ") freed\n"; }
};

void take_ownership(std::unique_ptr<Widget> w) {
  std::cout << "take_ownership now owns " << w->name << '\n';
}  // w dies here -> Widget freed here. The callee took over the lifetime.

int main() {
  std::cout << "1) make_unique + automatic cleanup:\n";
  {
    auto a = std::make_unique<Widget>("A");  // heap-allocated, owned by `a`
    std::cout << "  using " << a->name << " (use -> just like a pointer)\n";
  }  // <-- `a` goes out of scope HERE; ~Widget runs automatically. No delete.
  std::cout << "  (back in main -- A was freed at the brace)\n\n";

  std::cout << "2) move-only -- one owner, always:\n";
  auto b = std::make_unique<Widget>("B");
  // auto c = b;            // <-- WON'T COMPILE: copy would mean two owners
  //                        //     (that's the double-free bug). Try it: the
  //                        //     error names the deleted copy constructor.
  auto c = std::move(b);    // transfer ownership b -> c. b is now empty.
  std::cout << "  after move: b is " << (b ? "non-null" : "EMPTY")
            << ", c owns " << c->name << '\n';

  std::cout << "3) hand ownership to a function (move into the call):\n";
  take_ownership(std::move(c));      // c is emptied; callee frees on return
  std::cout << "  after the call: c is " << (c ? "non-null" : "EMPTY")
            << " -- the Widget is already gone\n\n";

  std::cout << "4) raw access when an API needs it (borrow, don't own):\n";
  auto d = std::make_unique<Widget>("D");
  Widget* raw = d.get();   // a NON-owning view -- d still owns it
  std::cout << "  borrowed raw view of " << raw->name
            << "; raw points at the same Widget d owns: "
            << (raw == d.get()) << '\n';
  // do NOT delete `raw` -- d owns it and will free it at end of main.

  std::cout << "end of main -- D freed automatically below:\n";
  return 0;
}
