# L17 — Make it trustworthy: testing demos

Material for Lecture 17 (Thu 2026-06-18). Everything here builds and runs in
the **Multipass `ec327` Ubuntu box** (g++ 15, the student environment).

Two layers of demo:

- **No-deps layer** — a 60-line teaching framework (`microtest.hpp`) so you
  can see a test framework is *not* magic, plus a pytest black-box harness.
- **Real-framework layer** — the same tests in **doctest**, **Catch2**, and
  **GoogleTest**, the libraries you'd actually reach for.

```
microtest.hpp     a ~60-line header-only test framework (build-to-learn doctest)
leap.hpp          the unit under test: is_leap_year(int)  (pure, edge-case rich)
test_leap.cpp     leap tests on microtest          -> all GREEN
roman.hpp         a second unit with a DELIBERATE bug (no subtractive notation)
test_roman.cpp    roman tests on microtest         -> 2 cases RED (the bug)
leap_doctest.cpp  leap tests in doctest            -> GREEN
roman_doctest.cpp roman tests in doctest           -> RED (shows expected vs actual)
leap_catch2.cpp   leap tests in Catch2 v3          -> GREEN
leap_gtest.cpp    leap tests in GoogleTest         -> GREEN
leapcli.cpp       a CLI wrapper around is_leap_year
test_leapcli.py   pytest driving ./leapcli via subprocess  -> black-box tests
```

## Packages (install once in the Multipass box)

These are **new packages** for the student VM — documented here, and added to
the Lab 2 / tooling setup so the box has them:

```sh
sudo apt-get update
sudo apt-get install -y doctest-dev catch2 libgtest-dev python3-pytest
```

| Package | Provides | Versions tested (Ubuntu 26.04) |
|---|---|---|
| `doctest-dev` | `<doctest/doctest.h>` (header-only) | 2.4.12 |
| `catch2` | Catch2 v3 headers + `libCatch2`, `libCatch2Main` | 3.7.1 |
| `libgtest-dev` | `<gtest/gtest.h>` + `libgtest`, `libgtest_main` | 1.17.0 |
| `python3-pytest` | `pytest` | 9.0.2 (Python 3.14) |

## 1. No-deps: the teaching framework (`microtest.hpp`)

```sh
g++ -std=c++20 -Wall -Wextra test_leap.cpp  -o test_leap  && ./test_leap   # GREEN: 11 checks, 0 failed
g++ -std=c++20 -Wall -Wextra test_roman.cpp -o test_roman && ./test_roman  # RED: 4 failed on purpose
```

Open `microtest.hpp` alongside — a framework is just (a) a registry of named
cases, (b) `CHECK` macros that record pass/fail with file+line, (c) a runner
that returns nonzero on failure (so CI sees red). doctest/Catch2/GoogleTest
are this idea, industrial-strength.

## 2. Real frameworks: same tests, three libraries

**doctest** (header-only — nothing to link; the course's default pick):

```sh
g++ -std=c++20 -Wall -Wextra leap_doctest.cpp  -o leap_doctest  && ./leap_doctest    # SUCCESS
g++ -std=c++20 -Wall -Wextra roman_doctest.cpp -o roman_doctest && ./roman_doctest   # FAILURE (the bug)
```

The roman run is the best illustration of why a real framework beats hand-rolled
asserts — it prints the **expected vs actual** values:

```
roman_doctest.cpp:35: ERROR: CHECK( roman_to_int("IV") == 4 ) is NOT correct!
  values: CHECK( 6 == 4 )
```

`6 == 4` tells you instantly the function summed `I+V` instead of subtracting.

**Catch2 v3** (link the library, which also supplies `main`):

```sh
g++ -std=c++20 -Wall -Wextra leap_catch2.cpp -o leap_catch2 -lCatch2Main -lCatch2 && ./leap_catch2
```

**GoogleTest** (link `gtest` + its `main`; needs `-pthread`):

```sh
g++ -std=c++20 -Wall -Wextra leap_gtest.cpp -o leap_gtest -lgtest -lgtest_main -pthread && ./leap_gtest
```

All three express the *same three ideas* — named cases, check macros
(`CHECK`/`REQUIRE`, `EXPECT_*`/`ASSERT_*`), a runner. Pick one; the habit
transfers.

## 3. pytest as an alternative harness (black box)

Compile the CLI, then test the *program* from the outside in Python:

```sh
g++ -std=c++20 -Wall -Wextra leapcli.cpp -o leapcli
python3 -m pytest -v test_leapcli.py        # 8 passed
```

We never `#include` the C++ — we run `./leapcli 2000` and assert on its
**stdout and exit code**. Reach for this when the unit is a whole program with
a CLI contract, or when you're gluing languages together.

## Build / run everything

```sh
g++ -std=c++20 -Wall -Wextra test_leap.cpp     -o test_leap     && ./test_leap
g++ -std=c++20 -Wall -Wextra test_roman.cpp    -o test_roman    && ./test_roman      # 2 RED on purpose
g++ -std=c++20 -Wall -Wextra leap_doctest.cpp  -o leap_doctest  && ./leap_doctest
g++ -std=c++20 -Wall -Wextra roman_doctest.cpp -o roman_doctest && ./roman_doctest   # RED on purpose
g++ -std=c++20 -Wall -Wextra leap_catch2.cpp   -o leap_catch2 -lCatch2Main -lCatch2 && ./leap_catch2
g++ -std=c++20 -Wall -Wextra leap_gtest.cpp    -o leap_gtest  -lgtest -lgtest_main -pthread && ./leap_gtest
g++ -std=c++20 -Wall -Wextra leapcli.cpp       -o leapcli
python3 -m pytest -q test_leapcli.py
```

> Build everything **under sanitizers** in a real project —
> `-fsanitize=address,undefined` — so a passing test run also certifies no
> memory/UB bugs on those inputs.

Instructor notes / the roman fix: `SOLUTIONS.md` (not published).
