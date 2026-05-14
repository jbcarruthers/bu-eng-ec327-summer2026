# EC327 Lab 0 — Getting Started

*Ungraded. Do this before Lab 1.*

By the end of this lab you'll have a Linux VM running on your laptop, the course toolchain installed in it, the course repo cloned, and a "hello world" program compiled and run. Total time: 30-60 minutes including downloads, less if you've used VMs before.

There's nothing to submit. Lab 1 starts where this leaves off, so it's worth getting all the way to the green checkmark at the bottom before week 1. It's a hard prerequisite for Lab 1 — the first graded lab. Get it working before our first meeting if you possibly can; bring questions and unresolved errors to class.

---

## What you need

A **laptop** with a **charger**, brought to every class meeting. Phones and tablets are not sufficient — you'll be running a Linux VM, a C++ toolchain, and an editor side by side.

- ✓ MacBook (Apple Silicon or Intel), Linux laptop, or a Windows 10/11 laptop (full Windows, not S-mode)
- ✓ Microsoft Surface running full Windows 11 (the desktop kind)
- ✗ iPads, Android tablets, Chromebooks, or Windows-on-ARM (Snapdragon) machines

Budget about **25 GB of free disk** and **8 GB of RAM** (the VM gets 4 GB; the rest is your host OS + editor + browser).

If your laptop situation is uncertain — older hardware, a borrowed device, a repair in progress — talk to me in the first week. We'll figure something out. The early conversation is much easier than the late one.

---

## Why a VM?

Half this class is going to be Mac users, half Windows. Both can compile C++ locally — but the toolchain, the file paths, and the failure modes are different on every machine. Rather than spend the term saying "but on my Mac…" / "but in PowerShell…", we standardize on one thing: **a small Ubuntu Linux VM that we all share**. Everyone's environment matches what I demo on the projector and what Lab graders test against.

The other reason — a thread that comes back in L21 — is **blast radius**. When Lab 1's `infinite.cpp` spins forever, killing the VM doesn't touch your Mac. When you `rm` something you shouldn't, it's recoverable. The VM is your first encountered *isolation boundary*; learning to think in terms of "where does this run, what can it reach" starts here.

---

## Before you start: open a terminal on your laptop

Most of what follows happens at a **terminal prompt** — a window where you type commands instead of clicking. Every step below assumes you have one open.

**macOS**: press `Cmd+Space` (Spotlight), type `terminal`, press Enter. Drag the Terminal icon out of the Dock and **right-click → Options → Keep in Dock** if you want it there permanently. Mac doesn't pin Terminal by default — most people add it on their first day with the machine.

**Windows**: press the Windows key (or click Start), type `powershell`, right-click **Windows PowerShell** and choose **Run as administrator**. That's the terminal you need for the WSL2 install in section 1. Once WSL2 is installed, you'll mostly use **Ubuntu** (also from the Start menu) instead — that's your Linux environment.

If you've never used a terminal before, that's expected — Lab 1 covers Unix shell skills properly. For Lab 0, every command you need is spelled out; copy-paste is fine.

> **Reference**: [`admin/terminal_cheatsheet.md`](../../../admin/terminal_cheatsheet.md) — the dozen-or-so commands that show up in this lab, one line each. Open it in another tab and consult as you go.

---

## 1. Install the VM tool for your OS

### macOS — Multipass

```
brew install --cask multipass
```

If you don't have Homebrew yet: install it from <https://brew.sh> first (one command, takes a few minutes).

Then **download `ec327_setup.yaml` from the course website** (it also lives next to this file in the lab directory, but you don't have the repo yet). Save it somewhere you can find it — `~/Downloads` is fine for now. This is a *cloud-init* file: it tells the VM what to install the moment it boots, so you don't hand-install the toolchain.

### Windows — WSL2

WSL2 ships with modern Windows. In the PowerShell window you opened above (running as Administrator), run:

```
wsl --install -d Ubuntu-24.04
```

Reboot when it tells you to. After the reboot, find the **Ubuntu** app in the Start menu — that's your Linux environment. First launch asks you for a username + password (anything will do — this is the VM's account, separate from your Windows account).

**Strongly recommended for Windows users — do these three things before continuing:**

1. **Pin Ubuntu to your taskbar.** Right-click the **Ubuntu** entry in your Start menu and pick **Pin to taskbar**. Now it's one click away, the same way Terminal sits in a Mac's Dock. Otherwise WSL feels hidden.
2. **Install Windows Terminal** from the Microsoft Store (free). Microsoft's modern terminal app — tabs, splits, copy-paste, native WSL profiles. Way better than the legacy console window. Once installed, pin it to your taskbar too.
3. **Plan to use VSCode + the WSL extension** for editing (section 8 below). It opens files that live *inside* WSL2 transparently — no copying between Windows and Linux. The single biggest UX win for Windows-WSL2 users.

> Windows users: WSL2 *is* your "VM" — there's no separate `multipass` step, and no cloud-init. Skip sections 2 and 4 entirely and substitute "your Ubuntu app" (or Windows Terminal connected to it) wherever the rest of the lab says "the VM."

### Native Linux

If you already run Linux on your laptop, you have two choices: install Multipass and follow the macOS path for full parity with the class, or skip the VM and install the toolchain natively (the apt block in section 5's Windows instructions works as-is). Either is fine — the day-to-day commands are identical from section 6 on.

---

## 2. Launch your `ec327` VM *(macOS only)*

From the directory where you saved `ec327_setup.yaml`:

```
multipass launch --name ec327 --cpus 2 --memory 4G --disk 20G \
  --cloud-init ec327_setup.yaml 24.04
```

This downloads Ubuntu 24.04 (~600 MB, once) **and runs the full toolchain install unattended**. Allow 10-15 minutes — your terminal returns to a prompt when it's done.

Enter the VM whenever you want with:

```
multipass shell ec327
```

You can leave the VM running between sessions. To verify it's alive: `multipass list` from the Mac shows it as `Running`.

---

## 3. Create your EC327 directory on the host

On your laptop (not inside the VM), make a directory where all your EC327 stuff lives:

```
mkdir -p ~/ec327
```

This is your dedicated course directory. Everything below goes here. **Don't dump course files in `~/Downloads`** — they'll get lost in the noise within a week.

---

## 4. Mount your directory into the VM *(macOS only)*

```
multipass mount ~/ec327 ec327:/home/ubuntu/ec327
```

This makes `~/ec327` on your Mac visible inside the VM at `/home/ubuntu/ec327`. Edits you make on the Mac show up immediately in the VM and vice versa — one tree, two views.

The first time you run this, Multipass may print `Enabling support for mounting` and spin for a few seconds — that's expected. The mount survives reboots, but Multipass occasionally forgets it after a full host reboot; if your files vanish from the VM side, just re-issue this command.

> Windows users: skip this. Your WSL2 Ubuntu home directory is already accessible from Windows via `\\wsl$\Ubuntu-24.04\home\<your-vm-user>\` (paste that into File Explorer). Make `~/ec327` *inside* WSL2 (`mkdir -p ~/ec327` from the Ubuntu shell), and access it from Windows through that path.

---

## 5. Toolchain — verify (macOS) or install (Windows)

### macOS — verify

The cloud-init from section 2 already installed everything. Open a shell inside the VM (`multipass shell ec327`) and confirm:

```
clang++ --version
g++ --version
git --version
cmake --version
```

All four should print version numbers without errors. If any of them is missing, the cloud-init failed partway through — see [troubleshooting](#if-you-get-stuck).

### Windows — install now

WSL2 doesn't use the cloud-init file, so run the equivalent once inside your Ubuntu shell:

```
sudo apt update
sudo apt install -y \
    build-essential clang clangd clang-format clang-tidy \
    cmake ninja-build gdb valgrind pkg-config git \
    python3 python3-pip python3-venv \
    man-db manpages manpages-dev less file tmux vim-tiny tree htop \
    curl wget zsh unzip

# uv + a Python venv (build scripts, the claude CLI, marimo)
curl -LsSf https://astral.sh/uv/install.sh | sh
source ~/.local/bin/env
uv venv ~/.venv/ec327

# Course shell defaults — match the macOS cloud-init.
cat >> ~/.bashrc <<'EOF'
source ~/.venv/ec327/bin/activate
export CXX=g++
export CXXFLAGS='-std=c++20 -Wall -Wextra -Wpedantic -Wshadow -O0 -g'
tidy() { clang-tidy --quiet "$@" -- -std=c++20 -Wall -Wextra -Wpedantic -Wshadow; }
EOF
```

Close and reopen the Ubuntu shell to pick up the changes, then verify:

```
clang++ --version
g++ --version
git --version
cmake --version
```

---

## 6. Clone the course repository

In a shell inside the VM (macOS) or your Ubuntu shell (Windows), in your EC327 directory:

```
cd ~/ec327
git clone https://github.com/jbcarruthers/bu-eng-ec327-summer2026.git course
```

You now have all the course materials under `~/ec327/course/` — slides, lab specs, demo code, the handouts I'll add as the semester goes.

### Important rule: never edit inside `course/`

The `course/` directory is the **course's** files. Treat it as read-only. Each week I'll push updates (new lab starter code, new demos, fixes), and you'll pull them with one command:

```
cd ~/ec327/course && git pull
```

If you've edited a file inside `course/`, `git pull` will refuse with a "your local changes would be overwritten" error. That refusal is the system telling you you've violated the rule — copy your changes elsewhere, then revert. (See [troubleshooting](#if-you-get-stuck) at the bottom.)

---

## 7. Create your work directory

This is where **your** code lives:

```
mkdir ~/ec327/work
```

When you want to play with a course file, copy it into `work/` first:

```
cp ~/ec327/course/inclass/first_prog.cpp ~/ec327/work/sandbox.cpp
```

Now edit `~/ec327/work/sandbox.cpp` to your heart's content. The original stays pristine and `git pull` keeps working forever.

> Why this two-directory pattern? It mirrors how every real software project works — the library is over there (`/usr/include/`, third-party deps, `std::`), your code is here. The rule that the filesystem enforces is the same rule good code maintains.

---

## 8. Set up your editor on the host

Editing happens on your **laptop**, not inside the VM. The VM has no GUI — it compiles and runs, that's it.

**Hard requirement: pick an editor with LSP (Language Server Protocol) support.** That's what lets the editor talk to `clangd` and give you live syntax checking, auto-formatting on save, and jump-to-definition. Without LSP, the editor is just a text box and you'll spend Lab 1 hunting typos that the editor could have flagged the instant you typed them.

**If you don't already have a preferred editor, install VSCode.** It's the default recommendation for this course because:

- **Free**, no nag screen, works the same on Mac and Windows.
- LSP-clangd works out of the box — install the official **clangd** extension by LLVM and you're done.
- **WSL extension** (Windows users) lets you edit files inside WSL2 transparently — paths and tools just work, no copying back and forth.
- Built-in terminal so you don't constantly switch windows.

Alternative editors that work just as well — pick what you can be productive in:

- **Sublime Text** — fast, clean, what I use in lecture. Free trial is unlimited (with an occasional nag screen). LSP via Package Control (`LSP` + `LSP-clangd`).
- **CLion** — JetBrains' full C++ IDE. Heavy but excellent. Free for students with a `.edu` email via JetBrains' student program.
- **Vim** / **Neovim** / **Emacs** — completely fine if you already use them. Set up via `coc-clangd` (Vim) or `lsp-mode` / `eglot` (Emacs).

Detailed per-editor install steps live in [`admin/editor_setup.md`](../../../admin/editor_setup.md) — covers Mac (Homebrew LLVM, PATH setup) and Linux/WSL2, plus the Sublime / VSCode / CLion / Vim / Emacs specifics.

Open your chosen editor on `~/ec327/work/` (your work directory). Save files there; they appear instantly in the VM through the mount.

---

## 9. First compile and run — the program I provide

Inside the VM:

```
cd ~/ec327/course/assignments/labs/lab0_getting_started
clang++ -std=c++20 -Wall -Wextra -Wpedantic -Wshadow hello.cpp -o /tmp/hello
/tmp/hello
```

You should see:

```
Hello, world! — EC327 toolchain is alive.
```

If you do, the toolchain is alive end-to-end: laptop (editor) → mount → VM (compile) → run → output.

> Why `-o /tmp/hello` and not just `clang++ hello.cpp`? Without `-o`, the compiler writes the binary to `a.out` in the current directory — which is inside the mount, so it'd show up on your Mac too (and in `git status` if you ever push). Putting binaries in `/tmp/` keeps the source tree clean. Lab 1 introduces a proper `Makefile` that handles this for you.

---

## 10. Now make it yours — copy, edit, recompile

You've compiled and run a program *I* wrote. Now do the same with your own copy, edited to say hello in your name. This is also the first real exercise of the copy-out-then-edit discipline from section 6.

Copy the file into your work directory:

```
mkdir -p ~/ec327/work/lab0
cp ~/ec327/course/assignments/labs/lab0_getting_started/hello.cpp ~/ec327/work/lab0/
```

Open `~/ec327/work/lab0/hello.cpp` in your editor (on your laptop). Find the line marked `// EDIT (Lab 0 step 10)` and replace `world` with your name. Save.

Back in the VM, compile and run *your* copy:

```
cd ~/ec327/work/lab0
clang++ -std=c++20 -Wall -Wextra -Wpedantic -Wshadow hello.cpp -o /tmp/hello
/tmp/hello
```

You should now see your name in the greeting:

```
Hello, Jane Doe! — EC327 toolchain is alive.
```

Three things just happened that matter for the rest of the term:

1. **Copy-out discipline in action.** You didn't touch anything in `course/`. The original `hello.cpp` is unchanged, `git pull` still works, your edit lives in `work/` where it belongs.
2. **End-to-end workflow proven.** Editor on the host → save → mount → compile in VM → run — the loop you'll use every day.
3. **The mount is live.** The instant you hit save in your editor, the file on the VM side is updated. No "sync" step, no "upload."

---

## 11. Weekly update workflow

Each week I'll add new material to the course repo. To pull it down:

```
cd ~/ec327/course
git pull
```

That's it. If it complains, see the troubleshooting section.

---

## Set up your screen for class

Tested over six weeks of EC441 in Spring 2026: **one role per desktop, swipe between them**. No picture-in-picture, no split-screen — those make text too small to read from the back of the room and harder for you to follow your own work.

**macOS — four Spaces.** Set up four desktop Spaces with Mission Control (`Control + ↑`, then `+` at the top right):

| Space | Role | App |
|---|---|---|
| 1 | Slides / handout | Preview or Skim, full-screen PDF |
| 2 | Terminal | iTerm2 or Terminal.app, one big pane, large font |
| 3 | Editor | Sublime / VSCode, one file open, large font |
| 4 | IDE (rare) | VSCode, when we explicitly need its debugger or Remote-SSH |

Swipe three fingers left/right to move between Spaces.

**Windows — virtual desktops.** Same idea: `Win + Tab` → "New Desktop". Switch with `Ctrl + Win + ←/→`. Same four roles.

**Linux — workspaces.** GNOME/KDE have workspaces; use them the same way.

Font size is not vanity — code that's readable from row 8 is readable for you, too. Terminal: 18-20pt. Editor: 18pt minimum.

---

## VM daily-use cheat sheet *(macOS / Multipass)*

| Task | Command |
|---|---|
| Start the VM | `multipass start ec327` |
| Open a shell | `multipass shell ec327` |
| Stop the VM | `multipass stop ec327` |
| Check status | `multipass list` |
| Re-mount the work directory | `multipass mount ~/ec327 ec327:/home/ubuntu/ec327` |
| Delete and start over | `multipass delete ec327 && multipass purge` |

The VM persists between reboots — you don't reprovision it. Just `multipass start ec327` then `multipass shell ec327`. (Windows/WSL2: open the Ubuntu app; `wsl --shutdown` from PowerShell stops it.)

---

## What success looks like

You're done with Lab 0 when **all** of these are true:

- [ ] `multipass list` (Mac) shows an `ec327` VM in `Running` state, *or* WSL2 Ubuntu opens cleanly (Windows).
- [ ] `~/ec327/course/` exists on your laptop and contains files like `inclass/first_prog.cpp`.
- [ ] Inside the VM, `clang++ --version`, `g++ --version`, `git --version`, `cmake --version` all print versions.
- [ ] Inside the VM, the smoke test (`clang++ ... hello.cpp -o /tmp/hello && /tmp/hello`) prints `Hello, world! — EC327 toolchain is alive.`
- [ ] You copied `hello.cpp` to `~/ec327/work/lab0/`, edited the greeting to say your name, recompiled, and the new run printed your name.
- [ ] You did NOT edit any file inside `~/ec327/course/`.
- [ ] Four desktop Spaces (or virtual desktops) set up for in-class use.

---

## If you get stuck

Lighter is more useful here than a 20-page recovery guide. Order of operations when something doesn't work:

1. **Read the error message.** Most of the time it tells you exactly what's wrong. (This is a course-long lesson, not a Lab 0 lesson, but it starts here.)
2. **Google the exact error text.** Surround it in quotes. Someone else has hit the same thing.
3. **Ask a generative AI** (Claude, ChatGPT, etc.). Paste the command you ran, the full error output, and your OS. Be specific. Lab 0 is a fine place to use AI — if you hit a wall on Multipass or WSL2, ask, then take notes on what worked.
4. **Ask me** — email, before class, after class. If steps 1-3 didn't get you there in 15 minutes, escalate; don't burn an evening.

A few specific recipes for the most common Lab 0 stumbles:

**"I edited inside `course/` and now `git pull` refuses."** Move your edits out and revert the course copy:

```
cd ~/ec327/course
cp inclass/whatever_you_edited.cpp ~/ec327/work/   # save your work
git checkout -- inclass/whatever_you_edited.cpp    # restore the original
git pull                                            # now works
```

**"Multipass `launch` fails with 'failed to obtain exit status'."** Usually a Hypervisor.framework permission issue. Restart the Multipass daemon: `sudo killall multipassd`, then `multipass list` relaunches it. If it persists, reboot the Mac and try again.

**"Multipass says the mount failed / permission denied."** Re-run with a fresh mount:

```
multipass umount ec327:/home/ubuntu/ec327
multipass mount ~/ec327 ec327:/home/ubuntu/ec327
```

If that still fails, restart the VM (`multipass restart ec327`) and try again.

**"WSL2 won't install — 'Virtualization not enabled'."** You need to enable virtualization in your laptop's BIOS/UEFI. Look for "Intel VT-x", "AMD-V", or "SVM Mode", enable it, save, and reboot Windows.

**"`clang++` not found inside the VM."** The cloud-init failed partway through (Mac), or you skipped section 5 (Windows). Re-run the install: `sudo apt update && sudo apt install -y build-essential clang clangd cmake gdb git`.

**"My editor still shows red squiggles on standard library types."** See `~/ec327/course/admin/editor_setup.md`, troubleshooting section.

---

## A note on the security thread

You'll see this come back through the term — most directly in L21, but in small callouts along the way.

What you set up today is a *sandbox*: a Linux environment that's isolated from your laptop's filesystem, your laptop's network, your personal files. Building and running software is a trust decision — the moment you `apt install` something, run a `Makefile`, or compile a C++ program, you're letting code do whatever your account can do. On your host, that includes your iCloud, your ssh keys, your browser cookies. Inside the VM, the same code only sees the VM. Anything that goes wrong in there — a runaway program, a misbehaving compiler, a process you can't kill — stays in there. Worst case, `multipass delete ec327 && multipass launch …` and you have a fresh one in two minutes. **Nothing in `~/Documents/` or your Photos library is at risk.**

This is the same idea as Docker containers, browser tabs, app sandboxes, and the permission prompts you see in Claude Code when it wants to run a command. The course's principle, in four words: **know your blast radius**. Before you grant some piece of code the ability to execute on your behalf, know what it can reach. The VM is your first concrete instance of that principle. We'll add more as we go.
</content>
</invoke>
