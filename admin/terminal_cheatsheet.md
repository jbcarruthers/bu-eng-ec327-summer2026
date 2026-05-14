# Terminal cheat sheet — the minimum for Lab 0

Just enough to copy/paste your way through Lab 0. **Lab 1** expands this into a proper Unix skill set (permissions, processes, redirection, pipes, search). For Lab 0, the tables below are enough.

Everything here works the same on macOS Terminal, Ubuntu in Multipass, and WSL2 Ubuntu — that's much of the point of standardizing on a Linux VM.

---

## Where am I? What's here? Where do I go?

| Command | What it does |
|---|---|
| `pwd` | **P**rint **w**orking **d**irectory — where am I right now? |
| `ls` | **L**i**s**t the files in this directory |
| `ls -la` | List **a**ll files (including hidden ones starting with `.`) in **l**ong format |
| `cd dirname` | **C**hange **d**irectory into `dirname` |
| `cd ..` | Go up one directory |
| `cd ~` | Go to your home directory (`cd` by itself does the same thing) |
| `cd -` | Go back to the previous directory |

---

## Making, copying, moving, removing files

| Command | What it does |
|---|---|
| `mkdir foo` | Create a directory named `foo` |
| `mkdir -p a/b/c` | Create nested directories `a/b/c`; **p**arents are made as needed and no error if they already exist |
| `cp src dst` | **C**o**p**y `src` to `dst`. If `dst` is a directory, copies *into* it. |
| `cp -r src dst` | **C**o**p**y a directory **r**ecursively (everything inside it too) |
| `mv old new` | **M**o**v**e (or rename) `old` to `new` |
| `rm file` | **R**e**m**ove a file. **Permanent. There is no Trash.** |
| `rm -r dir` | **R**ecursively delete a directory. **Be careful — no undo.** |

---

## Looking at file contents

| Command | What it does |
|---|---|
| `cat file` | Print the whole file to the terminal |
| `less file` | Read a file page by page. `q` quits, arrow keys scroll, `/text` searches forward. |
| `head file` | First 10 lines (use `head -n 20` for first 20) |
| `tail file` | Last 10 lines |
| `tail -f file` | **F**ollow a file as it grows — useful for logs |

---

## Running things

| Command | What it does |
|---|---|
| `./program` | Run `program` from the current directory. The `./` means "right here." |
| `program` | Run `program` if it's somewhere on your `$PATH` (Lab 1 explains this) |
| `Ctrl-C` | **Stop the currently-running program.** Press this when something is stuck or running forever. |
| `Ctrl-D` | End-of-input. Use it to quit a shell, or to tell a program reading from stdin "I'm done typing." |
| `exit` | Leave the current shell. Same as `Ctrl-D` in most cases. |

---

## Getting help

| Command | What it does |
|---|---|
| `man pwd` | The **man**ual page for `pwd` (or any command). `q` quits. |
| `pwd --help` | A shorter help summary that most commands support |

---

## Four tricks that pay off the first day

- **Tab completion.** Start typing a command, filename, or directory; hit `Tab`. The shell finishes the rest if it's unambiguous, or shows you the options. Use it constantly — it prevents typos and saves keystrokes.
- **Up arrow.** Re-run a previous command. Hit it multiple times to step back through your history.
- **`Ctrl-R`**: incremental search through your command history. Type a few characters of a past command; the shell finds it. `Ctrl-R` again to find an older match. `Enter` runs it; `Esc` to bail.
- **Don't type long paths.** Copy/paste from the lab text, or build them with tab completion. Typing `/home/ubuntu/ec327/course/inclass/` by hand is a recipe for typos.

---

## What this cheat sheet deliberately doesn't cover

Lab 1 properly covers:

- File permissions (`chmod`, `chown`, the `-rwxr-xr-x` notation)
- Processes (`ps`, `top`, `kill`, backgrounding with `&`, `jobs`, `fg`, `bg`)
- Redirection and pipes (`>`, `>>`, `<`, `|`, `2>`, `2>&1`)
- Searching (`grep`, `find`)
- Environment variables and `$PATH`
- The shell as a programming language (variables, conditionals, scripts)

If you're curious now, peek ahead — but you don't need any of it for Lab 0.
