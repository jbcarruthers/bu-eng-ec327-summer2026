# L15 — Generics & the STL: in-class demo + activity

Material for Lecture 15 (Tue 2026-06-16). Two parts.

## 1. Live demo — one template, every type

```sh
g++ -std=c++20 -Wall -Wextra max_of.cpp -o max_of && ./max_of
```
Shows a five-line function template instantiated for `int`, `double`, and
`std::string`. Point out: the compiler generates one function per `T` you
actually call with, and the only requirement here is "T supports `>`".

## 2. Activity — template-error triage

Three snippets that each **fail on purpose**, one per kind of template error:

| File | Error kind | Build |
|---|---|---|
| `triage1_sort.cpp`   | type missing `operator<` (sort)        | `g++ -std=c++20 -Wall -Wextra triage1_sort.cpp -o t1` |
| `triage2_deduce.cpp` | deduced conflicting types for `T`       | `g++ -std=c++20 -Wall -Wextra triage2_deduce.cpp -o t2` |
| `triage3_member.cpp` | `T` lacks a member (`.size()`)          | `g++ -std=c++20 -Wall -Wextra triage3_member.cpp -o t3` |

In pairs, for each: **(1)** predict the bug, **(2)** run the triage routine to
localize it from the wall (your line → first error → read as English → name the
missing capability), **(3)** fix it and **confirm the compiler is happy**. AI is
allowed on **triage3 only** — after a by-hand attempt, with disclosure, verified
by recompiling.

Each snippet's top comment carries a self-check hint. Fixes are in
`SOLUTIONS.md` (instructor-only — not published).
