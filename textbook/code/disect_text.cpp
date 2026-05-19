// disect_text.cpp -- EC327 Summer 2026
//
// Dissect a string with a for-loop and indexing: print each character
// next to its position. std::string::size() is unsigned, so the loop
// counter has type std::size_t to match.

#include <cstddef>
#include <iostream>
#include <string>

int main() {
  std::string s;

  std::cin >> s;

  for (std::size_t i = 0; i < s.size(); i++) {
    std::cout << i << ' ' << s[i] << '\n';
  }

  return 0;
}
