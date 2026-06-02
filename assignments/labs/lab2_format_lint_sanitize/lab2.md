# Lab 2 — Format · lint · sanitize

**EC327 Summer 2026 · graded · 2 points**

*Last revised: 2026-06-01.*

**Prerequisite: Lab 1 complete.** You should already be comfortable booting the
VM, opening a shell, building a single `.cpp` with `g++`, and capturing a
session with `script`. This lab assumes all of that and moves up one level:
from "can I build it?" to "is it *good*, and is it *safe*?"

Three tools, one theme. Every day, professional C++ goes through the same
three gates before anyone trusts it:

- **`clang-format`** — does it *look* like the rest of the codebase? (mechanical, no judgment)
- **`clang-tidy`** — does it contain known *bad patterns*? (a linter — judgment encoded as rules)
- **sanitizers** (`-fsanitize=address,undefined`) — does it actually *misbehave at runtime* in ways the compiler couldn't see? (instrumentation — catches bugs that compile cleanly and even appear to work)

Before any of that, **Part 0** opens the hood on something you've been doing on
faith since Lab 0: what *actually happens* when you type `g++ file.cpp`. You'll
watch the preprocessor, the compiler, and the linker as three separate steps.

Like Lab 1, the whole lab is **one captured `script` transcript**. The
`# learned:` one-liners are part of the transcript and part of the grade.

> **Security note.** Two of today's three tools are, at their core, security
> tools. `clang-tidy` ships checks whose entire purpose is catching
> exploitable patterns (use-after-move, not-null-terminated results, narrowing
> conversions that silently lose data). The sanitizers are how memory-safety
> bugs — the single largest category of security vulnerabilities in C and C++
> for the last forty years — get caught *before* an attacker finds them. The
> 2014 Heartbleed bug (CVE-2014-0160) was a heap over-read: code that compiled
> cleanly, passed review, ran in production on millions of servers, and leaked
> private keys to anyone who asked. AddressSanitizer, run on the test suite,
> would have caught it on the first malformed request. In Part 3 you'll watch
> ASan catch exactly that class of bug in a program that otherwise looks fine.

---

## Setup

Boot the VM, open a shell, and collect this lab's files into one working
directory. Everything you need is in this lab folder — copy it across the host
mount (Multipass) or through `\\wsl$` (WSL2), the same way you grabbed
`poem.cpp` in Lab 1.

```bash
multipass shell ec327          # macOS; or `wsl` on Windows
mkdir -p ~/work/lab2 && cd ~/work/lab2
ls                             # confirm these are here:
# .clang-format  .clang-tidy  CMakeLists.txt
# greet.h  greet.cpp  poem.cpp  messy.cpp  broken.cpp
```

The two files starting with a dot are the **course style configs** — the same
ones in the Tools & Development Environment zip on the course site. `ls` hides
them by default; `ls -a` shows them. The tools find them automatically by
walking *up* from the file being checked until they hit a `.clang-format` /
`.clang-tidy`, so as long as they sit next to your code, you don't name them on
the command line.

Now start recording:

```bash
script ~/work/lab2/lab2_transcript.txt
```

> Same rule as Lab 1: if you fumble a command, **don't restart**. Type
> `# fix:` and a short note, then redo it. We grade recovery, not theater.

---

## Part 0 — What actually happens when you compile

You've typed `g++ poem.cpp -o poem` dozens of times. It looks like one action.
It's really **three**, and you can run each one alone.

### The three stages

```bash
# 1. PREPROCESS (-E): expand #includes and macros, emit pure C++ text.
g++ -E poem.cpp | wc -l
# A 13-line file becomes tens of thousands of lines. Where did they come from?
g++ -E poem.cpp | tail -40        # the bottom is your actual code...
g++ -E poem.cpp | head -40        # ...the top is all of <iostream>, pulled in verbatim
```

`#include <iostream>` is not a function call. It is a literal **"paste the
entire contents of this file right here"** instruction, run before the compiler
sees anything. That one line drags in tens of thousands of lines of standard
library declarations. This is why a one-line "hello world" takes a noticeable
moment to compile — and why, in Part 2, you'll see `clang-tidy` care a lot about
*which* headers you include.

```bash
# 2. COMPILE (-c): turn ONE .cpp into ONE .o (machine code, not yet runnable).
g++ -std=c++20 -c poem.cpp        # produces poem.o
g++ -std=c++20 -c greet.cpp       # produces greet.o
ls *.o
file poem.o                       # "ELF ... relocatable" — object code, can't run yet
./poem.o                          # try it — "Permission denied" / "cannot execute"
```

Each `.o` is a *translation unit*: one source file compiled in isolation, with
no idea the others exist. `poem.o` calls `greeting()` but does not contain
it — it left a labeled hole that says "fill this in later."

```bash
# 3. LINK: stitch the .o files together into one executable, resolving the holes.
g++ poem.o greet.o -o poem
./poem
```

That final step is the **linker**. It found `greeting` (defined in `greet.o`),
matched it to the hole in `poem.o`, and produced a runnable program. This is the
whole reason `greet.h` exists: it's the *promise* (`greeting` will exist
somewhere) that lets `poem.cpp` compile before `greet.cpp` is even looked at.
Declaration in the header, definition in the `.cpp`, joined at link time.

To *see* the hole and the fill, break the link on purpose:

```bash
g++ poem.o -o poem_broken          # link poem.o ALONE, without greet.o
# undefined reference to `greeting(std::basic_string_view<char>)'
```

That "undefined reference" error is the single most common build error you will
ever hit in multi-file C++. You now know exactly what it means: **a hole no
`.o` filled.** Burn this into memory — it'll save you hours later.

**In your transcript:** write one `# learned:` line explaining, in your own
words, the difference between a *compile* error and a *link* error — and which
one "undefined reference to `greeting`" is.

### The same build, two ways to drive it

Doing all that by hand for two files is already tedious. For twenty files it's
unmanageable. `CMakeLists.txt` (you met it in Lab 1) describes the build once;
CMake runs the stages for you.

```bash
cat CMakeLists.txt              # read it — note `add_executable(poem poem.cpp greet.cpp)`
cmake -B build                  # configure
cmake --build build             # compile each .cpp, link them
./build/poem                    # same program, built by CMake instead of by hand
```

The payoff: when you edit only `greet.cpp`, `cmake --build build` recompiles
just `greet.o` and re-links — it doesn't rebuild `poem.o`. That incremental
behavior is why every real C++ project uses a build system instead of a wall of
`g++` commands.

---

## Part 1 — `clang-format`: make it *look* right

Open `messy.cpp` and read it. It **works** — it sums a short list and prints the
total. But it's painful to read: cramped spacing, braces crammed against code,
no consistent indentation.

```bash
cat messy.cpp
g++ -std=c++20 messy.cpp -o messy && ./messy      # confirm it works: sum = 108
```

`clang-format` rewrites code to match a style spec — purely mechanical, it never
changes behavior. First, **ask without touching anything**:

```bash
clang-format --style=file messy.cpp               # prints the reformatted version to the screen
```

(`--style=file` means "use the `.clang-format` next to the file." That's the
course style — Google base, 2-space indent, 80 columns, attached braces.)
Compare that output to the original on screen. Same code, dramatically more
readable.

Now the **dry-run check** — the form you'll use in CI and before every commit.
It changes nothing; it just reports whether the file already conforms:

```bash
clang-format --style=file --dry-run --Werror messy.cpp ; echo "exit=$?"
# non-zero exit + a list of lines that violate the style
```

Then **actually rewrite the file** with `-i` ("in place"):

```bash
clang-format --style=file -i messy.cpp
cat messy.cpp                                      # now properly formatted
clang-format --style=file --dry-run --Werror messy.cpp ; echo "exit=$?"   # 0 — nothing left to fix
g++ -std=c++20 messy.cpp -o messy && ./messy       # STILL prints sum = 108 — behavior unchanged
```

That last line is the whole point: formatting is safe. It moved every brace and
space, and the program does exactly what it did before.

**In your transcript:** write one `# learned:` line naming one specific thing
`clang-format` changed that you would *not* have thought to do by hand.

---

## Part 2 — `clang-tidy`: catch *bad patterns*

Formatting is about looks. **Linting is about substance.** `clang-tidy` reads
your code the way the compiler does and flags constructs that are legal C++ but
are known to be bug-prone, outdated, or against the course style. The course
`.clang-tidy` is a curated set (read the comments at its top — `cat .clang-tidy`)
anchored on the Google C++ Style Guide.

Run it. clang-tidy needs to know how the file is compiled, which you pass after
a `--`:

```bash
clang-tidy messy.cpp -- -std=c++20
```

You'll get **four** findings on `messy.cpp`. Read each one — the check name in
brackets (e.g. `[modernize-use-nullptr]`) is a searchable, permanent name:

1. `[google-build-using-namespace]` — **`using namespace std;`**. This is the
   one that matters most in this course. **Hard rule, Summer 2026: `using
   namespace std;` never appears in course code.** It drags every name in
   `std` into your file, which is how you get a mystery collision between your
   `distance` and `std::distance` three weeks from now. clang-tidy will *not*
   auto-fix this — you fix it by hand (next step).
2. `[modernize-use-nullptr]` — `NULL` is a C-era macro; C++ has the real keyword
   `nullptr`.
3. `[modernize-loop-convert]` — an index-counting `for` loop that should be a
   range-based `for`.
4. `[readability-simplify-boolean-expr]` — `if (finished == true)` is just
   `if (finished)`.

Three of those four are mechanical, and clang-tidy can apply them itself:

```bash
clang-tidy --fix messy.cpp -- -std=c++20          # auto-applies the fixable findings
clang-tidy messy.cpp -- -std=c++20                # re-run: only the using-namespace one remains
```

Now fix the last one **by hand**: delete the `using namespace std;` line and
qualify the standard names explicitly — `std::vector`, `std::cout`, `std::endl`.
(This is the disciplined alternative the course teaches: write `std::` out, or
add a narrow `using std::cout;` if it gets noisy — never the blanket directive.)
Re-run until clang-tidy is **silent**, re-format, and confirm the program still
works:

```bash
# (edit messy.cpp in your host editor: remove `using namespace std;`, add std:: )
clang-tidy messy.cpp -- -std=c++20                # silence = clean
clang-format --style=file -i messy.cpp            # tidy --fix can disturb layout; re-format
g++ -std=c++20 messy.cpp -o messy && ./messy       # sum = 108 — unchanged
cp messy.cpp fixed.cpp                             # this clean version is a deliverable
```

**In your transcript:** write one `# learned:` line explaining, in your own
words, *why* `using namespace std;` is banned here — not "because the rule says
so," but the actual problem it causes.

---

## Part 3 — Sanitizers: catch what *runs* wrong

Formatting and linting are **static** — they read your code without running it.
Some of the worst bugs in C++ are invisible until the program executes with the
wrong data, and they compile with zero warnings. `broken.cpp` is one of those.

First, build and run it **normally** and watch it *not* complain:

```bash
cat broken.cpp
g++ -std=c++20 -Wall -Wextra -O0 broken.cpp -o broken_normal      # NO warnings
./broken_normal
# wrapped = -2147483648
# data[4] = 16
echo $?                                                            # 0 — "success"
```

That is the trap. The compiler said nothing. The program ran, printed numbers,
and exited 0. But both numbers are *garbage produced by undefined behavior* —
the C++ standard says the program was allowed to do literally anything, and
"print a plausible-looking wrong number" is the most dangerous outcome because
nobody notices.

Now rebuild with the **sanitizers** — instrumentation the compiler weaves into
the binary to check every arithmetic op and every memory access at runtime:

```bash
g++ -std=c++20 -g -O0 -fsanitize=address,undefined broken.cpp -o broken
./broken
```

Two things fire, in order:

- **UBSan** (UndefinedBehaviorSanitizer) catches the first bug and prints a
  one-line report, then keeps going:
  ```
  broken.cpp:24:21: runtime error: signed integer overflow:
      2147483647 + 1 cannot be represented in type 'int'
  ```
  `INT_MAX + 1` overflows. In the normal build that silently wrapped to a
  negative number; here it's named, with the exact file and line.

- **ASan** (AddressSanitizer) catches the second bug — a write one element past
  the end of a 4-element heap array — and **stops the program cold** with a
  detailed report:
  ```
  ==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x...
  WRITE of size 4 at 0x... thread T0
      #0 ... in main broken.cpp:32
  0x... is located 0 bytes to the right of 16-byte region ...
      allocated by thread T0 here:
      #1 ... in main broken.cpp:30     <-- the `new int[4]`
  ```
  Read it top to bottom: **what** (heap-buffer-overflow, a WRITE of 4 bytes),
  **where** (`broken.cpp:32`, the loop body), and **what memory** (0 bytes past
  a 16-byte block — i.e. `int[4]` — allocated at `broken.cpp:30`). That is a
  complete bug report the compiler could not have given you, because the bug
  depends on the loop *running*.

You can also let CMake build the sanitizer version, via the toggle in this
lab's `CMakeLists.txt`:

```bash
cmake -B build-san -DENABLE_SANITIZERS=ON
cmake --build build-san
./build-san/broken                                # same two reports
```

**Capture the full ASan report in your transcript** — that captured output is
the deliverable for this part. Then, for credit, **fix `broken.cpp`**: change
the loop bound `i <= 4` to `i < 4`, and guard or remove the overflow, until
`./broken` (sanitized) runs clean with no reports. Keep the fixed version.

> **Why this matters beyond the lab.** ASan and UBSan are not academic toys —
> they run in the continuous-integration pipelines at Google, Mozilla, the Linux
> kernel, and essentially every serious C++ shop, on every commit. The mental
> model to leave with: *the absence of a compiler warning is not evidence of
> correctness.* In a memory-unsafe language, you buy that evidence by running
> your tests under instrumentation.

**In your transcript:** write one `# learned:` line about the difference between
a bug the **compiler** catches and a bug a **sanitizer** catches — and why
`broken.cpp` compiled with no warnings at all.

---

## Stop recording

```bash
exit                            # ends the script session
ls -lh lab2_transcript.txt
head -20 lab2_transcript.txt
```

If the file is empty or tiny, you never started `script` at the top — redo the
lab with it active.

---

## What to submit

A single zip or folder containing:

1. **`lab2_transcript.txt`** — the full captured session, including your
   `# learned:` lines, the Part 0 stage commands, the `clang-tidy` findings, and
   the **full ASan report from Part 3**.
2. **`fixed.cpp`** — your cleaned `messy.cpp`: clang-format leaves it unchanged
   and clang-tidy reports nothing, and it still prints `sum = 108`.
3. **`broken.cpp`** — your *fixed* version that runs clean under
   `-fsanitize=address,undefined`.
4. **`lab2_ai_disclosure.md`** — 3–10 lines naming which commands, errors, or
   sanitizer reports you asked an AI about, and whether it helped. "Pasted the
   ASan report into Claude to decode line `#1`, it pointed me at the `new int[4]`
   — that's how I found the allocation" is a good entry. "Did not use AI" is also
   valid.

---

## Grading rubric (2 points)

| Component                                                        | Weight |
|-----------------------------------------------------------------|--------|
| Part 0 stages captured (preprocess / compile / link, incl. the deliberate link error) | 0.5 |
| `fixed.cpp` is format-clean **and** tidy-clean, output preserved | 0.5 |
| Full ASan/UBSan report captured; `broken.cpp` fixed to run clean | 0.5 |
| `# learned:` lines specific + AI disclosure present and substantive | 0.5 |

**Late policy.** Sliding scale: `grade · (1 − h/(2H))` where `h` is hours late
and `H` is hours to the next class meeting. Gradual penalty, not a cliff.

---

## Optional: things to try if you finished early

These don't count for points but are worth seeing:

```bash
# See assembly, the stage after compile and before object code:
g++ -std=c++20 -S poem.cpp -o poem.s && head -40 poem.s

# What symbols does an object file export / leave as holes?
nm greet.o | grep greeting          # 'T' = defined here, 'U' = undefined (a hole)
nm poem.o  | grep greeting

# A second clang-tidy pass with EVERY check on (the firehose — see why we curate):
clang-tidy --checks='*' messy.cpp -- -std=c++20 2>&1 | head -40

# Leak detection: ASan also finds memory you allocate and never free.
# Add a `new int[10]` with no matching delete to a scratch file and watch.
```

If a sanitizer report stumps you, bring the transcript to office hours — the
file:line in the report is exactly where we'll start reading.

### Look under the hood: `cmake --build` is really `make`

Back in "two ways to drive it" you ran `cmake --build build` and took the
incremental rebuild on faith. Here's the reveal: on your Ubuntu box, `cmake -B
build` doesn't compile anything itself — it **generates a `Makefile`** and drops
it in `build/`. `cmake --build build` just runs `make` on that file.

```bash
ls build/Makefile          # CMake wrote a real Makefile here
make -C build              # identical to `cmake --build build`  (-C = "cd there first")
```

`make`'s whole job is to rebuild only what's *stale*, and it decides staleness
by comparing file **timestamps**. You can watch it reason — and ask it what it
*would* do before it does anything:

```bash
touch greet.cpp            # bump greet.cpp's mtime so it looks newer than greet.o
make -C build -n           # DRY RUN: prints the recompile + relink it WOULD run, runs nothing
make -C build              # do it for real: only greet.o rebuilds, then poem re-links
make -C build              # again: "Nothing to be done" — everything is up to date
```

Two flags worth keeping in your pocket:

```bash
make -C build -B           # FORCE: rebuild everything, ignore timestamps (no editing needed)
make -C build -j$(nproc)   # PARALLEL: compile independent .o files across all your cores
```

`touch` (bump one file's clock) and `-B` (ignore the clock entirely) are the two
ways to force a build: reach for `touch` when you want *one* target redone, `-B`
when you want the lot.

One caveat so you don't over-generalize: this `make` connection holds because
CMake's default **generator** on Ubuntu is "Unix Makefiles." CMake is really a
*meta*-build tool — install Ninja and configure with `cmake -B build -G Ninja`
and it writes a `build.ninja` instead, so `cmake --build build` drives `ninja`.
Same front door, different engine in the garage. That indirection is exactly why
the course drives builds through `cmake --build` rather than `make` directly:
one command, whatever's underneath.
