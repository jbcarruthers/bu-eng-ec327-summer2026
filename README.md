# EC327 — Introduction to Software Engineering

**Summer 2026 · Boston University · Department of Electrical and Computer Engineering**

This repository holds the student-facing course materials for EC327: lab specs,
in-class demo code, handouts, and tooling configuration. New material is pushed
here through the term.

---

## Getting the repo

You clone this once, during **Lab 0**, into your course directory:

```
git clone https://github.com/jbcarruthers/bu-eng-ec327-summer2026.git course
```

Then each week, pull updates with:

```
cd ~/ec327/course && git pull
```

**Never edit files inside this clone.** It's the course's copy — treat it as
read-only. Copy anything you want to modify into your own `work/` directory
first. Lab 0 explains the why and the how.

---

## Start here

### Lab 0 — Getting Started *(ungraded, do before week 1)*

Stand up your development environment: a Linux VM, the course toolchain, an
LSP-capable editor, and a first compile-and-run.

- **[assignments/labs/lab0_getting_started/README.md](assignments/labs/lab0_getting_started/README.md)** — the walkthrough
- [`hello.cpp`](assignments/labs/lab0_getting_started/hello.cpp) — the smoke-test program
- [`ec327_setup.yaml`](assignments/labs/lab0_getting_started/ec327_setup.yaml) — Multipass cloud-init (macOS VM provisioning)
- [`dotfiles/`](assignments/labs/lab0_getting_started/dotfiles/) — optional `.bashrc` / `.gitconfig` / `.vimrc` starter set

### Lab 1 — Unix + first build *(week 2, graded)*

Guided tour of the Unix shell, framed around the workflow you'll use every day:
edit on the host, build and run in the Linux shell. Two short programs, one
captured transcript submission.

- **[assignments/labs/lab1_unix_first_build/lab1.md](assignments/labs/lab1_unix_first_build/lab1.md)** — the lab handout
- [`poem.cpp`](assignments/labs/lab1_unix_first_build/poem.cpp), [`infinite.cpp`](assignments/labs/lab1_unix_first_build/infinite.cpp), [`CMakeLists.txt`](assignments/labs/lab1_unix_first_build/CMakeLists.txt) — starter source

### Textbook *(draft, in progress)*

The draft EC327 textbook lives at [`textbook/`](textbook/). Two editions and
every C++ program named in the book:

- [`EC327-week1-preview.pdf`](textbook/EC327-week1-preview.pdf) — chapters 1–3 only, the publishable subset (~40 pages)
- [`EC327-textbook-draft.pdf`](textbook/EC327-textbook-draft.pdf) — full draft including stubs (~120 pages)
- [`textbook/code/`](textbook/code/) — `.cpp` source for every program in the chapters (`quadratic0.cpp` through `grade_v5.cpp` today)

The textbook is also served on the course website at
<https://curl.bu.edu/ec327/summer2026/textbook>.

### Reference

- [`admin/terminal_cheatsheet.md`](admin/terminal_cheatsheet.md) — the minimum terminal commands for Lab 0
- [`admin/editor_setup.md`](admin/editor_setup.md) — per-editor clangd setup (VSCode / Sublime / CLion / Vim / Emacs)

### Tooling configs

The repo root ships `.clangd`, `.clang-tidy`, and `.clang-format`. Any editor
with LSP support picks these up automatically — you get the same warnings and
formatting the course uses. See `admin/editor_setup.md`.

---

## License

Code is MIT-licensed; course documents are CC BY 4.0. See [`LICENSE`](LICENSE)
and [`NOTICE`](NOTICE).
