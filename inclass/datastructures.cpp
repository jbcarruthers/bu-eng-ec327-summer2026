// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: interactive data-structure designer — recursive Q&A walker that emits a C++ type expression.
// Topic: decomposition
// Category: demo
// Lecture: L12
// Origin: summer_2024/inclass/datastructures.cpp (modernized 2026)

// Walks the user through type-design decisions and emits a C++ type
// expression (e.g., std::vector<std::map<int, std::string>>). Used as
// a live demo in EC327 L12 (linked structures + maps), and as a
// candidate student-extension exercise.
//
// What this is meant to demonstrate, beyond the answer it produces:
//
//   * Recursion: a data structure is recursive in its own definition
//     (a vector contains some other data structure, which may itself
//     be a vector, struct, map, ...). The program shape mirrors the
//     type shape.
//
//   * Mutual recursion: get_data_structure() handles "many things vs
//     one thing." When it hits "one thing but a new compound type,"
//     it delegates to get_atomic_data(), which itself loops calling
//     get_data_structure() to design each property's type. f calls g
//     calls f -- the somewhat rare paradigm that requires a forward
//     declaration. See the big comment above the forward decl below.
//
//   * Problem decomposition by data shape: the program *is* the
//     container decision tree (the L12 handout in runnable form).
//
//   * Small modern-C++ idioms: std::format, std::optional,
//     std::string_view, anonymous namespace, structured bindings,
//     ranges algorithms, enum class.

#include <algorithm>
#include <array>
#include <format>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

constexpr int INDENT = 2;

// ---------- I/O helpers ------------------------------------------

// Indent and print a prompt. Trailing space if it ends in '?',
// otherwise newline -- so questions read inline with the answer.
void prompt(std::string_view s, int indent = 0) {
  const std::string spaces(indent, ' ');
  if (not s.empty() and s.back() == '?') {
    std::cout << spaces << s << ' ';
  } else {
    std::cout << spaces << s << '\n';
  }
}

// Parse a yes/no token. nullopt on anything we don't recognize.
std::optional<bool> parse_yesno(std::string_view tok) {
  static constexpr std::array<std::string_view, 5> yeses{"y", "Y", "yes", "Yes",
                                                         "YES"};
  static constexpr std::array<std::string_view, 5> noes{"n", "N", "no", "No",
                                                        "NO"};
  if (std::ranges::find(yeses, tok) != yeses.end()) {
    return true;
  }
  if (std::ranges::find(noes, tok) != noes.end()) {
    return false;
  }
  return std::nullopt;
}

// Re-prompt until we get a recognized yes/no. EOF unwinds as "no".
bool ask_yes(int indent = 0) {
  std::string tok;
  while (std::cin >> tok) {
    if (const auto v = parse_yesno(tok)) {
      return *v;
    }
    prompt("I don't understand. y/n?", indent);
  }
  return false;
}

bool ask_no(int indent = 0) {
  return not ask_yes(indent);
}

// Read a whitespace-delimited word from stdin.
std::string read_token() {
  std::string s;
  std::cin >> s;
  return s;
}

// Read an integer, re-prompting on bad input.
int read_int(int indent = 0) {
  int n = 0;
  while (not(std::cin >> n)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    prompt("Not an integer. Try again:", indent);
  }
  return n;
}

// ---------- Type-design decision tree ----------------------------
//
// MUTUAL RECURSION between get_data_structure() and get_atomic_data().
//
//   get_data_structure  (top of the tree)
//     |
//     +-- many items --> array / vector / map / set / ...
//     |                     |
//     |                     +-- recurses on element / key / value type
//     |
//     +-- one item   --> get_atomic_data()
//                           |
//                           +-- primitive  --> int / double / char / ...
//                           |
//                           +-- new type   --> for each property:
//                                                recurse back into
//                                                get_data_structure().
//
// Because each function names the other in its body, we need a forward
// declaration. This is the canonical "f calls g calls f" pattern --
// rarer than plain recursion and worth recognizing on sight.

std::string get_data_structure(int indent = 0);  // forward decl

// ---------- Atomic types -----------------------------------------

std::string get_real(int indent) {
  prompt("Specialized need for small storage, speed-over-accuracy,", indent);
  prompt("or compatibility with old systems?", indent);
  return ask_yes(indent) ? "float" : "double";
}

std::string get_integer(int indent) {
  prompt("Could it be a particularly large integer (billions+)?", indent);
  if (ask_yes(indent)) {
    return "std::int64_t";
  }

  prompt("Does it need to match a bit pattern of a fixed size?", indent);
  if (ask_no(indent)) {
    return "int";
  }

  prompt("How many bits in the pattern?", indent);
  return std::format("std::uint{}_t", read_int(indent));
}

std::string get_number(int indent) {
  prompt("Is it an integer?", indent);
  if (ask_yes(indent)) {
    return get_integer(indent);
  }

  prompt("Could it be a complex number?", indent);
  if (ask_yes(indent)) {
    return "std::complex<double>";
  }

  return get_real(indent);
}

// The g-calls-f half of the mutual recursion: this is where we may
// recurse back into get_data_structure() (when designing a new
// compound type and asking for each property's type).
std::string get_atomic_data(int indent) {
  prompt("Your data is now atomic -- one indivisible thing.", indent);

  prompt("Is it a number?", indent);
  if (ask_yes(indent)) {
    return get_number(indent);
  }

  prompt("Is it a character?", indent);
  if (ask_yes(indent)) {
    return "char";
  }

  prompt("Two values only (true/false, on/off)?", indent);
  if (ask_yes(indent)) {
    return "bool";
  }

  prompt("A small fixed set of named values?", indent);
  if (ask_yes(indent)) {
    return "enum class /* TODO: name the enumerators */";
  }

  // None of the primitives fit -- design a new compound type.
  prompt("OK, we need a new compound data type.", indent);
  prompt("Should its properties be publicly accessible?", indent);
  const std::string_view kind = ask_yes(indent) ? "struct" : "class";

  prompt("What is this type named?", indent);
  const std::string name = read_token();

  prompt(std::format("Let's add properties of {} one at a time.", name),
         indent);
  std::vector<std::pair<std::string, std::string>> properties;
  while (true) {
    prompt("Add another property?", indent + INDENT);
    if (ask_no(indent + INDENT)) {
      break;
    }

    // <-- MUTUAL RECURSION: design each property's type by re-entering
    // the top of the decision tree. A property can itself be a
    // vector, a map, another struct, etc.
    const std::string type = get_data_structure(indent + INDENT);

    prompt("What is this property named?", indent + INDENT);
    const std::string field = read_token();
    properties.emplace_back(type, field);
  }

  std::string out = std::format("\n{} {} {{\n", kind, name);
  for (const auto& [type, field] : properties) {
    out += std::format("  {} {};\n", type, field);
  }
  out += "};";
  return out;
}

// ---------- Containers -------------------------------------------

std::string get_array(int indent) {
  prompt("How many items?", indent);
  const int n = read_int(indent);
  prompt("Now design what goes inside the array.", indent);
  const std::string inner = get_data_structure(indent + INDENT);
  return std::format("std::array<{}, {}>", inner, n);
}

std::string get_sequence(int indent) {
  prompt("Now design what goes inside the vector.", indent);
  const std::string inner = get_data_structure(indent + INDENT);
  // Special case: vector-of-char is std::string.
  if (inner == "char") {
    return "std::string";
  }
  return std::format("std::vector<{}>", inner);
}

std::string get_associative(int indent) {
  prompt("Does ordering of keys matter (sorted iteration, range queries)?",
         indent);
  const bool ordered = ask_yes(indent);
  const std::string_view prefix = ordered ? "std::" : "std::unordered_";

  prompt("Just membership (a set), or key-to-value (a map)?", indent);
  prompt("  Type 'set' or 'map':", indent);
  const std::string kind = read_token();

  if (kind == "set") {
    prompt("Element type:", indent);
    const std::string elem = get_data_structure(indent + INDENT);
    return std::format("{}set<{}>", prefix, elem);
  }

  // Default to map for anything other than "set" (including "map").
  prompt("Key type:", indent);
  const std::string key = get_data_structure(indent + INDENT);
  prompt("Value type:", indent);
  const std::string val = get_data_structure(indent + INDENT);
  return std::format("{}map<{}, {}>", prefix, key, val);
}

// ---------- Top of the tree --------------------------------------
//
// The f-calls-g half of the mutual recursion: in the "one item"
// branch we delegate to get_atomic_data(), which may turn around and
// call us back for each property of a new compound type.
std::string get_data_structure(int indent) {
  std::cout << '\n';
  prompt("Designing your data structure -- a few questions.", indent);

  prompt("More than one thing in the data?", indent);
  if (ask_no(indent)) {
    return get_atomic_data(indent);
  }

  prompt("Is the number of items fixed at compile time?", indent);
  if (ask_yes(indent)) {
    return get_array(indent);
  }

  prompt("Are items linear and accessed by position?", indent);
  if (ask_yes(indent)) {
    return get_sequence(indent);
  }

  return get_associative(indent + INDENT);
}

}  // anonymous namespace

int main() {
  const std::string type = get_data_structure();
  std::cout << "\nYour data type is:\n" << type << '\n';
  return 0;
}
