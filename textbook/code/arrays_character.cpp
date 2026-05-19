// arrays_character.cpp -- EC327 Summer 2026
//
// Two ways to hold text: a C-style char array (fixed size, ends at a
// '\0' terminator) and std::string (grows, knows its own length, and
// supports + and member functions). The contrast is the point -- reach
// for std::string; the char array is here so you recognize it.

#include <cstddef>
#include <iostream>
#include <string>

int main() {
  // --- C-style char array ---
  char greeting[10] = "Hello,";  // 6 chars + '\0', remaining slots zero-filled

  std::cout << greeting << '\n';

  // A char array is just chars. Index it to see each one -- including the
  // '\0' terminator (ASCII 0) and the zero-filled slots past it.
  for (int i = 0; i < 10; i++) {
    std::cout << "greeting[" << i << "] = '" << greeting[i] << "'  ascii "
              << static_cast<int>(greeting[i]) << '\n';
  }

  greeting[0] = 'h';  // individual elements can be changed
  std::cout << "changed: " << greeting << '\n';

  // --- std::string ---
  std::string name = "Eric";
  std::string othername("Jason");  // constructor-style initialization

  for (std::size_t j = 0; j < name.size(); j++) {
    std::cout << "name[" << j << "] = " << name[j] << '\n';
  }

  // Strings do things char arrays cannot: + concatenates, .append() and
  // .substr() are member functions.
  std::string full = std::string(greeting) + " " + name;
  full.append("!");
  std::cout << othername << " says: " << full << '\n';
  std::cout << "first 5 chars: " << full.substr(0, 5) << '\n';

  return 0;
}
