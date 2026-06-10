// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: std::map vs std::unordered_map -- lookup by key, the four access idioms.
// Topic: associative-containers
// Category: demo
// Lecture: L12

// L12, part 2. Associative containers: lookup by KEY, not by position.
//
//   std::map           -- balanced tree; keys kept SORTED; O(log n) ops.
//   std::unordered_map -- hash table; NO order; O(1) average ops.
//
// Reach for unordered_map by default (faster); reach for map when you want
// sorted iteration or range queries. This is one branch of the container
// decision tree (datastructures.cpp / the L12 handout).
//
// Two things to take away: the frequency-count idiom, and the FOUR ways to
// get at an element -- because they differ in what they do to a MISSING key.

#include <format>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

void count_words() {
  const std::vector<std::string> words{"red",   "blue", "red",
                                       "green", "blue", "red"};

  // The idiom: ++freq[w] default-constructs the value (int -> 0) the first
  // time a key is seen, then increments. One line replaces a manual
  // find / insert / update dance.
  std::map<std::string, int> freq;
  for (const auto& w : words) {
    ++freq[w];
  }

  // A std::map iterates in SORTED key order for free -- it is a tree.
  std::cout << "word frequencies (std::map -- sorted by key):\n";
  for (const auto& [word, n] : freq) {  // structured binding (L3)
    std::cout << std::format("  {:<6} {}\n", word, n);
  }
}

void access_methods() {
  std::unordered_map<std::string, int> ages{
      {"Ada", 36}, {"Alan", 41}, {"Grace", 85}};

  std::cout << "\nfour ways to access (they differ on a MISSING key):\n";

  // 1. operator[] -- convenient, but INSERTS a default if the key is absent.
  //    Reading a missing key with [] silently creates it (value 0). Footgun.
  std::cout << std::format("  ages[\"Ada\"]       = {}\n", ages["Ada"]);

  // 2. .at(key) -- checked: throws std::out_of_range if absent. Use it when
  //    the key MUST be there (the same .at() vs [] discipline as L3 vectors).
  std::cout << std::format("  ages.at(\"Alan\")    = {}\n", ages.at("Alan"));

  // 3. .contains(key) -- C++20, a clean membership test that never inserts.
  std::cout << std::format("  contains \"Grace\"?  {}\n",
                           ages.contains("Grace"));
  std::cout << std::format("  contains \"Bob\"?    {}\n", ages.contains("Bob"));

  // 4. .find(key) -- iterator to the element, or .end() if absent. The way
  //    to look up AND use the value without paying for a second lookup.
  if (const auto it = ages.find("Grace"); it != ages.end()) {
    std::cout << std::format("  find(\"Grace\")      -> {} is {}\n", it->first,
                             it->second);
  }
}

}  // namespace

int main() {
  count_words();
  access_methods();
  return 0;
}
