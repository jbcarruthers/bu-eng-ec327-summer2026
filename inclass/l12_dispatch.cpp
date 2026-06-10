// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: table-driven design -- a long if/else chain is a map in disguise.
// Topic: table-driven-design
// Category: demo
// Lecture: L12

// L12, part 3.
//
//   "Show me your flowcharts and conceal your tables, and I shall continue
//    to be mystified. Show me your tables, and I won't usually need your
//    flowcharts; they'll be obvious."          -- Fred Brooks, 1975
//
// A long if/else (or switch) chain that maps an input to a result is almost
// always a TABLE wearing a costume. Replace it with a map lookup and the
// data becomes explicit, the control flow becomes one line, and adding a
// case becomes a one-row edit instead of a new branch. This is the moment
// "when a long if/else chain wants to become a map."

#include <format>
#include <iostream>
#include <string_view>
#include <unordered_map>

namespace {

// BEFORE: the control flow IS the data. The mapping is buried in branches,
// and every new HTTP status code means a new `if`.
std::string_view status_text_ifelse(int code) {
  if (code == 200) {
    return "OK";
  }
  if (code == 404) {
    return "Not Found";
  }
  if (code == 418) {
    return "I'm a teapot";
  }
  if (code == 500) {
    return "Internal Server Error";
  }
  return "Unknown";
}

// AFTER: the data IS the data. The table is obvious at a glance, the lookup
// is one line, and adding a code is a one-row edit. `static` so the table is
// built once, not on every call.
std::string_view status_text_table(int code) {
  static const std::unordered_map<int, std::string_view> kStatus{
      {200, "OK"},
      {404, "Not Found"},
      {418, "I'm a teapot"},
      {500, "Internal Server Error"},
  };
  const auto it = kStatus.find(code);
  return it != kStatus.end() ? it->second : "Unknown";
}

}  // namespace

int main() {
  std::cout << "code   if/else                table-driven\n";
  for (const int code : {200, 404, 418, 500, 999}) {
    std::cout << std::format("{:<5}  {:<21}  {}\n", code,
                             status_text_ifelse(code), status_text_table(code));
  }
  return 0;
}
