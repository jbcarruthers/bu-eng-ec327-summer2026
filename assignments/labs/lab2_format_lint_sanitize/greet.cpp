// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: definition of greeting() — the second TU in the Lab 2 build.
// Topic: anatomy
// Category: canonical
// Lecture: lab2

#include "greet.h"

#include <string>
#include <string_view>

std::string greeting(std::string_view name) {
  return "a greeting from " + std::string(name) +
         ", a machine that learned to speak today.";
}
