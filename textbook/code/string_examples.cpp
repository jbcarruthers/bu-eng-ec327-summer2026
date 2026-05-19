// string_examples.cpp -- EC327 Summer 2026
//
// The same idea -- a sequence of characters -- held four different ways:
// std::string, a C-style char array, std::array<char>, and
// std::vector<char>. std::string is the one to reach for; the others
// are here for contrast.

#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

int main() {
  // --- std::string ---
  std::string s{"abcDEFGH"};
  std::cout << "string:  " << s << '\n';

  // Index every character with [].
  for (std::size_t i = 0; i < s.size(); i++) {
    std::cout << i << ' ' << s[i] << '\n';
  }

  // Characters are numbers: += 1 shifts each one up by one code point.
  for (char& ch : s) {
    ch += 1;
  }
  std::cout << "shifted: " << s << '\n';

  // Strings concatenate with +.
  s = s + "XYZ";
  std::cout << "joined:  " << s << '\n';

  // --- a C-style char array: fixed size, cannot be reassigned ---
  char sc[7] = "abcdef";
  std::cout << "array:   " << sc << '\n';

  // --- std::array<char>: fixed size, but a real object with .size() ---
  std::array<char, 6> sa{};
  for (std::size_t i = 0; i < sa.size(); i++) {
    sa[i] = s[i];
  }
  for (char ch : sa) {
    std::cout << ch;
  }
  std::cout << '\n';

  // --- std::vector<char>: no fixed size; push_back appends ---
  std::vector<char> sv;
  sv.reserve(sa.size());  // reserve the space we know we will need
  for (std::size_t i = 0; i < sa.size(); i++) {
    sv.push_back(s[i]);
  }
  for (char ch : sv) {
    std::cout << ch;
  }
  std::cout << '\n';

  return 0;
}
