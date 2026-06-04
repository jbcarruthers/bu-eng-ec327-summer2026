// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: shared_ptr = reference-counted SHARED ownership; weak_ptr observes
//          without owning and breaks reference cycles. "Use unique_ptr until
//          you can prove you need shared_ptr."
// Topic: modern-pointers
// Category: demo
// Lecture: L11
// Build: g++ -std=c++20 -O0 -g l11_shared.cpp -o l11_shared

// unique_ptr is the default: ONE owner. Sometimes ownership genuinely IS
// shared -- several parts of a program each need the object kept alive, and
// none of them is "the" owner. shared_ptr handles that with a reference count:
// the resource is freed when the LAST shared_ptr to it dies (count hits 0).
//
// The catch: two shared_ptrs that point at each other form a CYCLE -- each
// keeps the other's count at 1 forever, so neither is ever freed (a leak a
// garbage collector would catch, but C++ won't). weak_ptr is the fix: it
// observes the object without bumping the count, so it doesn't keep it alive.
#include <iostream>
#include <memory>
#include <string>
#include <utility>  // std::move

struct Person {
  std::string name;
  std::shared_ptr<Person> best_friend;        // strong: bumps the count
  std::weak_ptr<Person>   acquaintance;        // weak: observes, no count
  explicit Person(std::string n) : name(std::move(n)) {
    std::cout << "  Person(" << name << ") created\n";
  }
  ~Person() { std::cout << "  ~Person(" << name << ") destroyed\n"; }
};

int main() {
  std::cout << "1) reference count rises and falls with sharing:\n";
  auto a = std::make_shared<Person>("Ada");
  std::cout << "  count = " << a.use_count() << "  (just a)\n";
  {
    // The copy IS the lesson here: it bumps the reference count. (The lint
    // would otherwise suggest a const& -- which would defeat the demo.)
    auto also_a = a;  // NOLINT(performance-unnecessary-copy-initialization)
    std::cout << "  count = " << a.use_count() << "  (a + also_a, both name "
              << also_a->name << ")\n";
  }                           // also_a dies -> count drops
  std::cout << "  count = " << a.use_count() << "  (also_a gone)\n\n";

  std::cout << "2) weak_ptr observes without owning:\n";
  std::weak_ptr<Person> watcher = a;          // does NOT raise the count
  std::cout << "  count still " << a.use_count() << " (weak_ptr doesn't own)\n";
  if (auto locked = watcher.lock()) {         // promote to use it safely
    std::cout << "  locked: " << locked->name
              << " (count temporarily " << a.use_count() << ")\n";
  }
  std::cout << '\n';

  std::cout << "3) a CYCLE leaks; weak_ptr breaks it:\n";
  {
    auto x = std::make_shared<Person>("X");
    auto y = std::make_shared<Person>("Y");
    x->best_friend = y;        // strong x -> y
    y->acquaintance = x;       // WEAK y -> x  (use best_friend here instead
                               // and neither destructor would ever run!)
    std::cout << "  X count = " << x.use_count()
              << ", Y count = " << y.use_count() << '\n';
  }  // both freed here precisely BECAUSE one edge is weak:
  std::cout << "  (both destroyed at the brace -- no leak)\n";
  return 0;
}
