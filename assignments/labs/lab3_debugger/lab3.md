# Lab 3 — GDB

**EC327 Summer 2026 · graded · 2 points**

*Released 2026-06-03. Due Wed 2026-06-17 23:59; late window to Thu 2026-06-18 23:59.*

**Prerequisite: Lab 1 complete** (you can boot the VM, build a `.cpp` with
`g++`, and capture a session). Labs 1 and 2 taught you to make code *build*,
*look right*, and *not misbehave under sanitizers*. This lab is about the next
question: **when a program is wrong, how do you find out *why*?**

The answer is not "add `cout` everywhere and recompile." It's **GDB**, the GNU
debugger — a tool that freezes your program mid-run and lets you inspect it like
a paused video: read any variable, read raw memory, see the chain of function
calls that got you here, stop the instant a condition becomes true, and step one
line at a time. Every C++ shop uses it (or its cousin LLDB); the
breakpoint/backtrace/watchpoint vocabulary you learn today transfers to the
debugger built into VS Code, CLion, and every IDE you'll ever touch.

You'll hunt **four classic bugs**, each chosen for a different GDB superpower:

| Program | Symptom | GDB tool you'll learn |
|---|---|---|
| `calclog` | prints garbage instead of a logarithm | **breakpoints** + `print &x` + `x` (examine memory) |
| `sheep` | "infinite" loop that isn't infinite | **watchpoints** (`watch i==0`) |
| `fibonacci` | segfaults on certain input | **backtrace** of a crash |
| `tichu` | throws an uncaught exception | **`catch throw`** + **conditional breakpoints** |

Keep the companion **[GDB reference guide](https://curl.bu.edu/ec327/summer2026/tools/gdb)**
(course site → Tools & Development Environment → GDB Reference) open in a browser
tab while you work — every command below is in it, with shortcuts.

> **Security note.** A debugger is a dual-use tool, and that's not a footnote —
> it's the whole reason the skill is valuable on both sides of the fence. The
> exact same GDB session that lets *you* watch a variable get corrupted is how a
> reverse engineer watches a closed-source binary handle a malformed input, how
> a malware analyst single-steps a sample to see what it does before it phones
> home, and how an exploit developer confirms they've overwritten a return
> address. Backtrace, examine-memory (`x`), and watchpoints — the three things
> you'll do today — are *the* core moves of binary analysis. You are learning
> the defender's and the attacker's most basic shared instrument. When `fib(-6)`
> segfaults in Part 3, you'll see a corrupted call stack; "what happens to the
> call stack when input goes somewhere the author didn't plan for" is the
> one-sentence summary of stack-smashing, the bug class behind decades of remote
> code execution. We are nowhere near writing exploits in this course — but
> notice that the instrument is the same.

---

## Setup

Boot the VM, open a shell, and collect this lab's files into one directory —
the four `.cpp` files and the `CMakeLists.txt`, copied across the host mount
(Multipass) or through `\\wsl$` (WSL2), the same way you grabbed files in Labs 1
and 2.

```bash
multipass shell ec327          # macOS; or `wsl` on Windows
mkdir -p ~/work/lab3 && cd ~/work/lab3
ls                             # confirm these are here:
# calclog.cpp  sheep.cpp  fibonacci.cpp  tichu.cpp  CMakeLists.txt
```

**GDB needs debug symbols.** A normal `g++ file.cpp -o file` strips out the
information GDB uses to map machine code back to *your* source — line numbers,
variable names, types. The `-g` flag keeps it. **Every build in this lab uses
`-g`**; forget it and GDB works in machine-code Braille instead of your source.

```bash
g++ -std=c++20 -g calclog.cpp -o calclog
g++ -std=c++20 -g sheep.cpp    -o sheep
g++ -std=c++20 -g fibonacci.cpp -o fibonacci
g++ -std=c++20 -g tichu.cpp    -o tichu
```

(Or `cmake -B build && cmake --build build` — the provided `CMakeLists.txt`
builds all four as a **Debug** configuration, which adds `-g` for you. Either
way, the binaries carry symbols.)

### Capturing your work: `gdb | tee`

Unlike Labs 1 and 2, you will **not** wrap the whole lab in one `script`
session. Instead you capture **one transcript per bug hunt** by piping GDB
through `tee`, which copies everything — your typed commands and GDB's
responses — into a file while still showing it on screen:

```bash
gdb calclog | tee debug_calclog.txt
```

You interact with GDB exactly as normal; `debug_calclog.txt` quietly fills up
with the entire session. Those four `debug_*.txt` files are your transcripts and
the bulk of your grade. (Why `tee` and not `script` here? Because GDB is the
only thing you're running — there's no surrounding shell activity worth
capturing, and a per-bug file keeps the four hunts cleanly separated.)

> If you fumble a command mid-session, **don't restart** — GDB doesn't care, and
> neither do we. A misremembered command and the recovery is more instructive
> than a suspiciously clean transcript. Type `# fix:` notes if you like; GDB
> treats `#`-lines as comments.

---

## Part 1 — Breakpoints + examine memory: `calclog`

`calclog.cpp` is supposed to print the natural log of a number. It doesn't:

```bash
./calclog
# *Log Finder*
# Please enter a number:10
# The natural logarithm of 10 is 3.04121e-314      <-- garbage, not 2.302585
```

`ln(10)` is about `2.3026`. That `3e-314` is the tell-tale signature of an
**uninitialized variable**: you're reading memory that was never written, so you
get whatever bit-pattern was lying there. We'll use GDB to prove exactly that.

Launch it under GDB, capturing to a file:

```bash
gdb calclog | tee debug_calclog.txt
```

At the `(gdb)` prompt:

```gdb
list                      # 1. see the source (note the two `lognum` declarations!)
break 14                  # 2. the line `double lognum = std::log(num);`
break 18                  # 3. the line that PRINTS lognum
info breakpoints          # 4. confirm both are set
run                       # 5. starts; enter 10 when prompted; stops at line 14
```

Now the key move. **Twice** — once stopped at line 14, then `continue` to line
18 — print both the *value* and the *address* of `lognum`:

```gdb
print lognum              # at bp 14
print &lognum             # the ADDRESS of this lognum
continue                  # run on to line 18 (the print line)
print lognum              # at bp 18
print &lognum             # the ADDRESS of this lognum
```

**You should see something impossible-looking: the two `&lognum` addresses are
different.** Same name, two different memory locations — because there are *two
different variables* both named `lognum`. The one declared inside the
`if` block on line 14 (`double lognum = …`) **shadows** the one declared on
line 9. `std::log(num)` is computed and stored into the *inner* `lognum`, which
then dies at the closing `}` on line 15. Line 18 prints the *outer* `lognum`,
which was never assigned — hence the garbage.

To drive the point home, read the *inner* address as a floating-point number
even though that variable is now out of scope. The `x` ("examine") command reads
raw memory; `x/gf ADDR` reads 8 bytes (`g` = "giant" = a `double`'s worth) and
interprets them as a float (`f`):

```gdb
x/gf <the inner &lognum address from line 14>
# 0x...:   2.3025850929940459        <-- there's your logarithm, hiding in dead memory
help x                    # read what every letter in `x/gf` means
```

**In `lab3_questions.txt`:** the one-word fix and the line it's on (Q1).

---

## Part 2 — Watchpoints: `sheep`

`sheep.cpp` claims to print multiples of 1000 *forever*. Run it — it stops:

```bash
./sheep | tail
# ...
# -3000
# -2000
# -1000
# 0            <-- and then it exits. So much for "forever."
```

The loop runs `while (i != 0)`. Something makes `i` come back around to `0`. A
`print`-after-every-iteration approach would scroll past tens of thousands of
lines. A **watchpoint** is the right tool: it stops the program the instant an
expression's value *changes* — no looping, no guessing.

```bash
gdb sheep | tee debug_sheep.txt
```

```gdb
break 12                  # the line `std::int16_t i = 1;`
run
info locals               # i and counter, before the loop
watch i==0                # STOP when (i==0) flips from false to true
continue                  # runs full speed until that moment, then halts
```

When the watchpoint fires, `i` has just become `0`. Inspect the neighborhood:

```gdb
print counter             # how many iterations did it take?
print i
info locals
```

The culprit is the **type**: `i` is a `std::int16_t` — a signed 16-bit integer,
range −32768…+32767. Incrementing past `32767` doesn't keep climbing; it
**overflows** and wraps to −32768, then climbs back through zero — and `i==0`
ends the loop. The "infinite" loop was bounded by the integer width all along.

> The same wrap-around, in a buffer-length or array-index calculation, is a
> classic security bug (an "integer overflow"). Picking a type too small for the
> values it will hold is not a stylistic nit — it's a correctness and safety
> decision.

**In `lab3_questions.txt`:** the largest value `i` reaches before the loop ends
(Q2).

---

## Part 3 — Backtrace of a crash: `fibonacci`

`fibonacci.cpp` computes Fibonacci numbers by recursion, and for sensible input
it works:

```bash
./fibonacci
# Enter a number x to calculate F_x :6
# F_6 = 13
# I used 25 function calls.
```

Now feed it `-6`:

```bash
./fibonacci
# Enter a number x to calculate F_x :-6
# Segmentation fault (core dumped)
```

A **segmentation fault** is the OS killing your program for touching memory it
isn't allowed to. You'll meet *many* of these. The single most useful thing GDB
does is tell you **exactly where** one happened and **how you got there** — the
`backtrace`.

```bash
gdb fibonacci | tee debug_fibonacci.txt
```

```gdb
run                       # enter -6 when prompted; it will crash inside GDB
# Program received signal SIGSEGV, Segmentation fault.
# fib (x=-...) at fibonacci.cpp:...
backtrace 10              # the top 10 frames — the most recent calls
backtrace -10             # the bottom 10 frames — the oldest calls
```

Read what the backtrace shows you: a tower of `fib` calling `fib` calling
`fib`, thousands deep. The base cases are `if (x==0)` and `if (x==1)` — neither
is ever true for a negative `x`, which only gets *more* negative
(`fib(x-1) + fib(x-2)`). The recursion never bottoms out, each call eats a slice
of the **call stack**, and when the stack is exhausted the program dies. That's
a **stack overflow** — the literal kind the famous website is named after.

You've now *located* the bug. Fixing it (reject negative input, or define the
base case as `x <= 1`) is the easy part once GDB has shown you the tower.

**In `lab3_questions.txt`:** the line number of the mistake that makes the
positive-input sequence wrong, too (Q3).

---

## Part 4 — `catch throw` + conditional breakpoints: `tichu`

`tichu.cpp` builds a deck for the card game Tichu and deals four hands. It
crashes — but not with a segfault this time:

```bash
./tichu
# ... prints the whole deck fine ...
# terminate called after throwing an instance of 'std::out_of_range'
#   what():  vector::at: __n (which is ...) >= this->size() (which is ...)
# Aborted (core dumped)
```

An **uncaught exception**. `std::vector::at()` does bounds-checking (unlike
`[]`) and **throws** `std::out_of_range` when you ask for an index it doesn't
have. Nobody caught it, so the program aborts. We'll catch it *in the debugger*
to freeze the program at the exact `throw`.

```bash
gdb tichu | tee debug_tichu.txt
```

First, just run and backtrace from the abort:

```gdb
run
backtrace                 # `bt` and `where` are the same command
```

Now stop at the *throw itself*, before the stack unwinds:

```gdb
catch throw               # a catchpoint: break the instant any exception is thrown
run                       # re-run; GDB halts AT the throw now
backtrace                 # cleaner — you're standing right where it broke
```

The backtrace points into `show_card()`. The bad call is one of the `.at()`
calls in that function. To watch it happen on the precise card that breaks, set
a **conditional breakpoint** — a breakpoint that only fires when an expression
is true:

```gdb
break tichu.cpp:59 if c==52       # line 59 is `cout << show_card(c)`; only when c==52
run
print c                   # 52
print deck                # inspect the data
step                      # step INTO show_card()  (vs. `next`, which steps over)
display c                 # auto-print c after every step from now on
next                      # step line by line...
next                      # ...watch which .at() call throws
```

Card `52` maps to no valid `(value, suit)` pair: `suits.at(52/13)` is
`suits.at(4)`, but `suits` holds only 4 elements (indices 0–3). `.at(4)` throws.

**Now the `[]`-vs-`.at()` lesson.** Copy the file and swap every `.at(i)` for
`[i]`:

```bash
cp tichu.cpp tichu_brackets.cpp
# edit tichu_brackets.cpp: change every `.at(x)` to `[x]`
g++ -std=c++20 -g tichu_brackets.cpp -o tichu_brackets
./tichu_brackets
```

It does **not** throw — `operator[]` does no bounds checking. But it doesn't
*work*, either: it reads past the end of the vector (undefined behavior) and
prints nonsense, or crashes differently, or appears to run. **That contrast is
the entire point.** `.at()` turned a silent memory error into a loud, located,
catchable exception. `[]` is faster precisely because it trusts you — and a
wrong index inside `[]` is the single most common bug you'll write this term
(your instructor checks the index expression *first* when grading). Reach for
`.at()` whenever an index might be out of range; reach for `[]` when you've
already proven it can't be.

**In `lab3_questions.txt`:** the out-of-range vector's name (Q4), and the last
line both `tichu` and `tichu_brackets` print before their behavior diverges
(Q5).

---

## Further questions

Open **`lab3_questions.txt`** (ships with the lab) and replace the placeholder
answers with your real ones. It asks five short questions, one per bug hunt plus
the `[]`-vs-`.at()` divergence. Line numbers count from 1.

---

## What to submit

Submissions are **a single `.zip` file**, uploaded at
[`curl.bu.edu/ec327`](https://curl.bu.edu/ec327). The zip must contain these
six files:

1. **`debug_calclog.txt`** — your Part 1 GDB session (breakpoints, the two
   different `&lognum` addresses, the `x/gf` reveal of the hidden logarithm).
2. **`debug_sheep.txt`** — your Part 2 session (the `watch i==0` catch and the
   value of `counter`/`i` when it fires).
3. **`debug_fibonacci.txt`** — your Part 3 session (the SIGSEGV and the
   `backtrace` of the recursion tower).
4. **`debug_tichu.txt`** — your Part 4 session (`catch throw`, the backtrace,
   and the conditional breakpoint stepping into `show_card`).
5. **`lab3_questions.txt`** — with all five answers filled in.
6. **`lab3_ai_disclosure.md`** — 3–10 lines naming which commands, errors, or
   GDB output you asked an AI about, and whether it helped. "Asked Claude what
   `x/gf` means before `help x` — it confirmed the g=giant/f=float decoding" is
   a good entry. "Did not use AI" is also valid.

### Building and checking your zip

The same helper from Labs 1–2, **`ec327_zip.py`**, ships in this folder:

```bash
python3 ec327_zip.py build lab3       # writes lab3_submission.zip + checks it
python3 ec327_zip.py check lab3       # re-check an existing zip any time
```

`build` collects the six required files, skips junk (`build/`, the compiled
binaries, …), and prints a present/missing checklist. You can also zip by hand —
the upload page only cares that it's one valid `.zip`, and it re-runs the same
checklist against the names *inside* your zip after you upload.

> Upload **one `.zip`**, not loose files and not a `.tar.gz` — the uploader
> rejects anything else.

---

## Grading rubric (2 points)

| Component                                                        | Weight |
|-----------------------------------------------------------------|--------|
| `debug_calclog.txt` + `debug_sheep.txt`: breakpoints, the two `&lognum` addresses, `x/gf`, and the `watch i==0` catch all captured | 0.5 |
| `debug_fibonacci.txt`: SIGSEGV reproduced under GDB and `backtrace` captured | 0.5 |
| `debug_tichu.txt`: `catch throw` + conditional breakpoint + step-into captured; `tichu_brackets` divergence understood | 0.5 |
| `lab3_questions.txt` answers correct + AI disclosure present and substantive | 0.5 |

**Late policy.** Sliding scale: `grade · (1 − h/(2H))` where `h` is hours late
and `H` is hours to the next class meeting. Gradual penalty, not a cliff.

---

## Optional: things to try if you finished early

These don't count for points but are worth seeing — and most are in the GDB
reference guide:

```gdb
# Single-step a recursion and watch the call stack grow/shrink:
#   break fib   /  run (enter 5)  /  bt full  /  continue ... repeat
# This is the exact demo from Lecture 8 — now you can drive it yourself.

# TUI mode: a split-screen source+command view. Toggle with Ctrl-X Ctrl-A,
# or start gdb with  -tui . Arrow keys scroll the source pane.

# `frame N` jumps to frame N of a backtrace; `up` / `down` walk it.
# `info args` / `info locals` print that frame's arguments and locals.

# `finish` runs until the current function returns and prints its return value.
# `until N` runs until line N — handy for getting past a loop without bp spam.
```

```bash
# Examine memory layouts (previews Lecture 9):
gdb tichu
(gdb) break main
(gdb) run
(gdb) next 5
(gdb) print deck
(gdb) print &deck
(gdb) x/8dw &deck        # read 8 decimal words starting at deck's address
```

If a GDB session stumps you, bring the `debug_*.txt` to office hours — the
file:line in the backtrace is exactly where we'll start reading.
