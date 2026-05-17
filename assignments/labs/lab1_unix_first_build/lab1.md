# Lab 1 — Unix + first build

**EC327 Summer 2026 · graded · 2 points**

*Last revised: 2026-05-11.*

**Prerequisite: Lab 0 complete.** If your VM (Mac) or WSL2 (Windows) isn't
standing up yet — toolchain installed, `hello.cpp` building — finish Lab 0
before starting here. Lab 0 is the one-time setup; Lab 1 is what you'll
actually use the shell for.

This lab is a guided tour of the Unix shell, framed around the workflow you
will use every day in this course: edit on the host, build and run in the
Linux shell. You'll write two short programs (`poem.cpp` and `infinite.cpp`),
build and run them several ways, redirect their I/O, control them as
processes, and finish with a 60-second taste of CMake. The whole lab is
**one captured shell transcript** — see "What to submit" at the end.

You will not memorize Unix commands by reading this document. You'll
memorize them by typing them, watching them, and explaining what you saw.
Where this document says "explain in one line what just happened," do
that — those one-liners are part of your transcript and part of the grade.

> **Security note.** Everything you do in this lab runs inside the VM, not
> on your Mac or Windows host. That's deliberate — every command in this
> lab is yours, but in the weeks ahead you'll start cloning repos,
> installing packages, and running AI-generated code. Each of those is a
> trust decision: `pip install foo` runs `foo`'s `setup.py` on your
> machine; `git clone` downloads a `Makefile` that can do anything `make`
> can do; `python x.py` is exactly as dangerous as you trust the author
> of `x.py` to be. The 2024 xz-utils backdoor (CVE-2024-3094) is the
> canonical recent example: a maintainer slipped a backdoor into a
> trusted compression library through ~2 years of legitimate-looking
> contributions, and it would have compromised every Linux SSH server if
> a Postgres engineer hadn't noticed `sshd` was 500 ms slower than it
> should be. Reading the code wouldn't have caught it. The VM is your
> blast-radius mitigation — get used to running risky things inside it.

---

## Setup

Boot the VM and open a shell.

**macOS / Multipass**
```bash
multipass start ec327
multipass mount ~/ec327-work ec327:/home/ubuntu/work   # if not already mounted
multipass shell ec327
```

**Windows / WSL2**
```bash
wsl                   # opens an Ubuntu-24.04 shell
```

Now start recording. The `script` command captures everything that happens
in a shell session — input, output, and timing — to a file. We'll grade
that file.

```bash
script ~/work/lab1_transcript.txt    # Multipass; on WSL2, drop the ~/work prefix
```

Your prompt will reappear. From this moment until you `exit` the script
session, every keystroke and every line of output is being saved.

> If you make a mistake mid-transcript, **don't start over**. Type `# fix:`
> followed by a brief comment, then redo the command. We'd rather see you
> recover than fake a clean run.

---

## Part 1 — Filesystem navigation (10 commands)

Type each of these. After each block, in one comment line in your shell
(`# what I just learned: ...`), say something specific you noticed.

```bash
pwd                  # where am I?
ls                   # what's here?
ls -l                # the long form
ls -la               # everything, including dotfiles
ls -lah              # human-readable sizes
cd ~                 # home directory
cd /                 # the root of the filesystem
cd /etc              # somewhere specific
cd -                 # back to where I was
cd                   # equivalent to cd ~
```

Now explore. From your home directory:

```bash
ls /                 # what lives at the root?
ls /usr/bin | head   # what executables ship with Ubuntu?
ls /usr/bin | wc -l  # how many?
file /usr/bin/g++    # what is that file?
file /etc/passwd     # how about this one?
```

Two looking-things-up commands you'll use forever:

```bash
man ls               # the manual page (press q to quit)
ls --help            # the inline help (shorter)
man man              # the manual for man itself
```

**In your transcript, after this section**, write one `# learned:` comment
naming **one** thing about absolute vs relative paths you didn't know an
hour ago.

---

## Part 2 — File operations and permissions

```bash
cd ~/work            # Multipass; on WSL2, cd to wherever your work tree lives
mkdir lab1_play
cd lab1_play

touch a.txt b.txt c.txt
ls

# Globs — the shell expands these before the command sees them
ls *.txt
ls [ab].txt

cp a.txt a_copy.txt
mv b.txt renamed.txt
ls

# Removing things — pay attention; there is no Trash
rm c.txt
rm a_copy.txt renamed.txt
ls
```

Now permissions. Every file has a permission string like `-rw-r--r--`:

```bash
ls -l a.txt
# -rw-r--r-- 1 ubuntu ubuntu 0 May 11 14:00 a.txt
#  └┬─┘└┬─┘└┬─┘
#   │   │   └── others
#   │   └────── group
#   └────────── owner
# Each triple is r-w-x (read, write, execute).
```

```bash
chmod +x a.txt          # mark it executable
ls -l a.txt
chmod -x a.txt          # un-mark
ls -l a.txt
```

`a.txt` is empty, so making it "executable" is meaningless — but you can do
it. The permission bit is the OS's gate; whether the file actually does
anything when you run it is a separate question. We'll come back to this.

> **Security note.** `chmod +x` is a grant. It tells the OS "this file is
> allowed to run as a program," and once it runs, the program can do
> anything your user account can do — read your files, write your files,
> open network connections, call any system call you can. The single bit
> is the only thing standing between "an arbitrary file on disk" and "a
> process with your authority." Inside the VM this risk is contained;
> outside, every time you download a script and run `chmod +x setup.sh
> && ./setup.sh`, you're granting the same authority to a stranger.
> Cybersecurity is its own course later on — for now, just notice the
> grant when you make it.

Clean up: `cd .. && rm -r lab1_play`.

**In your transcript:** write one `# learned:` comment about a glob you
tried that surprised you (it matched too much, too little, or matched
something you didn't expect).

---

## Part 3 — Build, run, and the meaning of `./`

Make sure `poem.cpp` is in your work directory. (Grab it from this lab
folder if you don't have it — copy it via the host mount on Multipass, or
drag it through `\\wsl$` on Windows.)

```bash
cd ~/work            # or wherever poem.cpp lives
ls poem.cpp          # confirm it's there
cat poem.cpp         # look at it
```

Now build it three ways. **First**, the simplest thing:

```bash
g++ poem.cpp
ls                   # what file appeared?
file a.out           # what kind of file is it?
./a.out               # run it
```

`a.out` is `g++`'s default output name (historical: "assembler output,"
from the original 1970s Unix). You can name the output whatever you like:

```bash
g++ poem.cpp -o poem
ls
file poem
./poem
```

**Why `./poem` and not just `poem`?** Try it:

```bash
poem
# bash: poem: command not found
```

When you type a bare command, the shell searches a list of directories
called `$PATH` — and the current directory is, by long convention, **not
on it**. The historical reason is a multi-user-server worry: if `.` were
on your PATH and you `cd`'d into `/tmp` on a shared machine, someone could
plant a malicious `ls` there and have you run it.

On your personal VM that risk is largely theoretical — you are the only
user, you don't `cd /tmp`. So this is a real engineering choice with a
real tradeoff, and you'll see your instructor in class running with `.`
on PATH because the day-to-day convenience of typing `hello` instead of
`./hello` wins for him. Either choice is defensible; make it consciously.

For the rest of this lab — and for most C++ work in this course — we'll
stick with `./poem`, because it makes the "this is the binary I just
built right here" intent unmistakable.

Investigate `$PATH`:

```bash
echo $PATH
# /home/ubuntu/.venv/ec327/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
which g++            # which g++ does the shell find?
which ls
type ls              # type tells you whether it's a binary, alias, function, builtin
type cd
which cd             # contrast: cd isn't a binary, it's a shell builtin
```

Optional aside — adding your own bin directory to PATH:

```bash
mkdir -p ~/bin
mv poem ~/bin/
ls ~/bin
poem                 # still not found — ~/bin isn't on PATH yet
export PATH="$HOME/bin:$PATH"
poem                 # now it works
which poem           # the shell now knows where to find it
mv ~/bin/poem .      # put it back so the rest of the lab still works
```

**In your transcript:** write one `# learned:` comment explaining, in your
own words, why typing `./poem` works but typing `poem` does not.

### Aside: `make` does the typing for you

Typing `g++ -std=c++20 -Wall -Wextra -O0 -g poem.cpp -o poem` gets old
fast. `make` is the standard Unix tool for "compile this thing, but only
if the source is newer than the binary." It has built-in rules that know
how to turn a `.cpp` into an executable using the variables `$CXX` and
`$CXXFLAGS`, which your shell already sets from the course dotfiles. So:

```bash
rm poem                          # start fresh
make poem                        # make figures it out
# g++ -std=c++20 -Wall -Wextra -O0 -g    poem.cpp   -o poem
make poem                        # second time: nothing to do
# make: 'poem' is up to date.
```

No `Makefile` in the directory — `make` infers the rule from the file
name. This works for **single-file programs**. The moment you split into
multiple `.cpp` files, you need an actual `Makefile` (or CMake, which
you'll see at the end of this lab). But for the everyday "I edited
`hello.cpp`, rebuild it" loop, `make hello` is all you need to type.

If `make poem` complains `g++: error: poem.cpp: No such file or
directory`, you forgot to `cd` to where `poem.cpp` lives.

---

## Part 4 — Redirection, pipes, and exit codes

Every running program has three open streams:

- **stdin** (file descriptor 0) — where input comes from
- **stdout** (fd 1) — where normal output goes
- **stderr** (fd 2) — where error messages go

By default all three are connected to your terminal. You can re-route any
of them.

```bash
# Send poem's stdout to a file
./poem > poem_out.txt
cat poem_out.txt

# Append rather than overwrite
./poem >> poem_out.txt
cat poem_out.txt        # poem appears twice now

# Discard stdout entirely
./poem > /dev/null
```

Stderr is separate from stdout:

```bash
ls /this_does_not_exist               # error goes to stderr
ls /this_does_not_exist > out.txt     # stdout redirected, but error still appears
ls /this_does_not_exist 2> err.txt    # NOW stderr is redirected
cat err.txt
ls / /this_does_not_exist > out.txt 2> err.txt   # both, to different files
ls / /this_does_not_exist > combined.txt 2>&1    # both, to the same file
```

Stdin is the input direction:

```bash
# wc counts lines/words/bytes from its stdin
wc poem_out.txt                        # passing a filename
wc < poem_out.txt                      # piping the file in via stdin
```

Pipes — connect one program's stdout to the next program's stdin:

```bash
./poem | wc                            # how many lines/words/bytes does the poem have?
./poem | grep machine                  # which lines mention "machine"?
./poem | grep -i program               # case-insensitive search for "program"
./poem | sort                          # poem sorted alphabetically
./poem | sort -r                       # reverse-sorted
ls /usr/bin | wc -l                    # how many executables on the system?
```

Exit codes — every command returns a number; `0` means success.

```bash
./poem
echo $?                                # 0
ls /this_does_not_exist
echo $?                                # nonzero
true; echo $?                          # 0 — the command literally named `true`
false; echo $?                         # 1 — same idea
```

The exit code is what lets you chain commands intelligently:

```bash
g++ poem.cpp -o poem && ./poem         # only run if the build succeeded
mkdir junk || echo "already exists"    # only print if mkdir failed
g++ poem.cpp -o poem ; ./poem          # run sequentially regardless
```

`tee` — split a stream so it both goes to a file AND to your terminal:

```bash
./poem | tee poem_seen.txt             # you see it on screen and it goes to the file
```

**In your transcript:** write one `# learned:` comment explaining what
`2>&1` does, in words a friend who has never seen it would understand.

---

## Part 5 — Processes and job control

Build `infinite.cpp`. It prints `tick 1`, `tick 2`, … forever, one per second.

```bash
g++ -std=c++20 infinite.cpp -o infinite
./infinite
# tick 1
# tick 2
# tick 3
# ...
```

Now stop it. Press **Ctrl-C** in the terminal.

```bash
# ^C
echo $?     # 130 — the convention: "killed by signal 2 (SIGINT)"
```

Ctrl-C sent the SIGINT signal to the running process. There are others:

```bash
./infinite
# ^Z          (Ctrl-Z) — suspends the process; you get your prompt back
jobs          # the suspended job is listed
bg            # resume it in the background — it keeps ticking but you have your shell
jobs
fg            # bring it back to the foreground
# ^Z again
bg
sleep 5; echo "five seconds passed"    # your shell still works while infinite ticks
```

While `infinite` is running in the background, find it:

```bash
ps                          # processes from THIS shell
ps -ef | head               # all processes on the system
ps -ef | grep infinite
pgrep infinite              # just the PID
top                         # interactive process viewer; q to quit
# (htop is installed too — same idea, prettier)
```

Send signals by name or number:

```bash
fg                          # foreground; ^Z to suspend; bg to background
kill %1                     # %1 = job number 1; default signal is SIGTERM (15)
jobs                        # gone
./infinite &                # & at the end of a command = start in background
jobs
kill -9 %1                  # SIGKILL — the unblockable one; "fall over now"
jobs
kill -l                     # list all signal names and numbers
```

> **The Ctrl-C vs `kill -9` distinction matters in your future career.**
> Ctrl-C / SIGTERM ask the program to stop. SIGKILL doesn't ask — the kernel
> just removes it. Use SIGTERM first; only use `-9` when a program has stopped
> responding.

**In your transcript:** write one `# learned:` comment about the difference
between "the process is in the background" (`bg`) and "the process is dead"
(`kill`).

---

## Part 6 — CMake cameo

You've now compiled `poem.cpp` by typing the full `g++` command. That works
for one file. For five files with options and dependencies, you'd want a
build system. The build system this course uses is **CMake**. You don't
need to learn it today; you just need to see it run.

Grab the `CMakeLists.txt` from this lab directory. Place it next to
`poem.cpp`. Then:

```bash
cmake -B build              # configure: read CMakeLists.txt, set up the build/ directory
ls build                    # look around
cmake --build build         # build: actually compile
ls build                    # find the new `poem` binary
./build/poem                # run it
```

That's it. We will return to CMake properly in Lab 2 (alongside
`clang-format` and `clang-tidy`) and again in lecture 18. For now, the
takeaway is that the same source file can be built either by a one-line
`g++` command or by `cmake -B build && cmake --build build` — and as
projects grow, the second path is the one that scales.

---

## Stop recording

```bash
exit                        # leaves the script session
# Script done, file is ~/work/lab1_transcript.txt
ls -lh lab1_transcript.txt  # confirm it exists
head -30 lab1_transcript.txt
```

If the file is empty or only a few lines long, you didn't actually start
`script` at the top. Re-do the lab from the top with `script` active.

---

## What to submit

A single zip or folder containing:

1. **`lab1_transcript.txt`** — the full captured session, with your `# learned:`
   comment lines interleaved.
2. **`poem.cpp`** — your copy (unmodified is fine; if you tweaked the poem,
   so much the better).
3. **`infinite.cpp`** — your copy.
4. **`CMakeLists.txt`** — the one you used in Part 6.
5. **`lab1_ai_disclosure.md`** — a short markdown file (3–10 lines) naming
   which commands or errors you asked an AI about during this lab, and what
   worked or didn't. "Used AI to explain `2>&1`, it gave a clear analogy
   I'd seen before — confirming, not learning new" is a valid entry. "Did
   not use AI" is also a valid entry.

---

## Grading rubric (2 points)

| Component                                  | Weight |
|--------------------------------------------|--------|
| Transcript captures all six parts          | 0.5    |
| `# learned:` comments are specific (not generic) | 0.5 |
| Programs build and run; CMake build works  | 0.5    |
| AI disclosure file present and substantive | 0.5    |

**Late policy.** Sliding scale: `grade · (1 − h/(2H))` where `h` is hours
late and `H` is hours to the next class meeting. Gradual penalty, not a
cliff.

---

## Optional: things to try if you finished early

These don't count for points but are useful to know:

```bash
history                     # everything you've typed in this shell
!42                         # re-run command #42 from history
!!                          # re-run the most recent command
!g                          # re-run the most recent command starting with g
Ctrl-R                      # interactive history search

find . -name "*.cpp"        # find C++ files under the current directory
grep -rn "tree" .           # find "tree" in every file under here, with line numbers
xargs                       # apply a command to many things (advanced)

tar -czf archive.tgz dir/   # bundle a directory into a tarball
tar -xzf archive.tgz        # un-bundle it
```

If anything in this lab broke and you want to discuss it, bring the
transcript to office hours — it'll have everything we need to retrace.
