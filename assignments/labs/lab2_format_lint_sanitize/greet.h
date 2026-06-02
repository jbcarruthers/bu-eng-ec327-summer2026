// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: declaration-only header for the Lab 2 two-file build (Part 0).
// Topic: anatomy
// Category: canonical
// Lecture: lab2

#ifndef GREET_H_
#define GREET_H_

#include <string>
#include <string_view>

// Returns a one-line greeting addressed to `name`.
// Declared here, defined in greet.cpp — the point of Part 0 is to watch the
// declaration (this header) and the definition (the .cpp) live in different
// translation units and get joined by the linker.
std::string greeting(std::string_view name);

#endif  // GREET_H_
