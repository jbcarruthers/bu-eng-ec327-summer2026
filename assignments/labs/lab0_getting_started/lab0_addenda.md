# EC327 Lab 0 — Addenda

Optional follow-ons to Lab 0's [`README.md`](README.md). You don't need any of
them to finish Lab 0 on the standard path — Multipass on Mac, WSL2 on Windows.
Reach for the one that matches your situation:

- **Addendum A — Installing VS Code.** Click-by-click, Mac and Windows. Read
  this if section 8 of the README told you to "install VSCode" and you want the
  steps spelled out.
- **Addendum B — macOS without Multipass.** A native-macOS fallback for when
  Multipass genuinely will not run. An escape hatch — *not* the recommended path.
- **Addendum C — Finding your files across WSL and Windows.** For Windows users:
  the two filesystems, how to find one from the other, and where your course
  files should actually live.

---

## Addendum A — Installing VS Code

Lab 0 recommends VS Code as the default editor: free, identical on Mac and
Windows, the smoothest `clangd` setup, and — for Windows users — the WSL
extension is the single biggest quality-of-life win you can get. This addendum
just gets the application onto your machine. The `clangd` *configuration*
(language server, format-on-save, the Homebrew-LLVM `PATH` fix) lives in
[`admin/editor_setup.md`](../../../admin/editor_setup.md); do that next.

### macOS

1. **Download** from <https://code.visualstudio.com> — click "Download for
   macOS". The Universal build covers both Apple Silicon and Intel. You get a
   `.zip`.
2. **Unzip** — Finder unzips it when you double-click. Drag
   `Visual Studio Code.app` into your `/Applications` folder. Don't run it from
   `~/Downloads` — it won't self-update cleanly from there.
3. **First launch** — macOS Gatekeeper warns "Visual Studio Code is an app
   downloaded from the Internet." Click **Open**.
4. **Install the `code` command** — open the Command Palette
   (`Cmd+Shift+P`), type `shell command`, and run **"Shell Command: Install
   'code' command in PATH"**. Now `code .` opens the current folder from any
   terminal. *Do this* — launching VS Code from a terminal is how it inherits
   your Homebrew-LLVM `PATH` (see `editor_setup.md` for why that matters).

> Homebrew shortcut: `brew install --cask visual-studio-code` does steps 1–3 in
> one command and installs into `/Applications` for you. You still want step 4.

### Windows

Install VS Code on **Windows itself — not inside Ubuntu/WSL.** A common Lab 0
mistake is running `sudo apt install code` in the Ubuntu shell. That is not how
it works: VS Code is a Windows application, and its WSL extension reaches *into*
Linux from the Windows side.

1. **Download** from <https://code.visualstudio.com> — click "Download for
   Windows". Pick the **User Installer** (the default offered) — it installs
   per-user and needs no administrator rights.
2. **Run the `.exe`.** On the "Select Additional Tasks" screen, leave **Add to
   PATH** ticked (on by default). The "Open with Code" context-menu entries are
   handy too.
3. **Launch** it from the Start menu. Pin it to your taskbar while you're there.

### The two extensions you need

Open the Extensions panel (`Cmd/Ctrl+Shift+X`) and install:

- **clangd** — published by **LLVM**. The C++ language server: live error
  squiggles, autocomplete, jump-to-definition, format-on-save. Required, Mac and
  Windows alike.
- **WSL** — published by **Microsoft**. **Windows users only.** Lets VS Code
  edit files *inside* WSL2 transparently. After installing it, click the blue
  `><` corner at the bottom-left → **"Connect to WSL"**, then **File → Open
  Folder** → `/home/<you>/ec327`. Paths, terminals, and tools all run Linux-side
  from then on — no copying back and forth.

The rest of the setup — disabling Microsoft's competing C/C++ IntelliSense
engine, turning on format-on-save, the Homebrew-LLVM `clangd.path` fix — is in
[`admin/editor_setup.md`](../../../admin/editor_setup.md). This addendum only
gets VS Code on disk.

> **Quick check:** open any `.cpp` file from the course repo. Within a few
> seconds `clangd` should start (watch for a "clangd: indexing" flicker in the
> status bar), and hovering over `std::cout` should show a tooltip. No red
> squiggles on standard-library names means it's working.

---

## Addendum B — macOS without Multipass *(escape hatch)*

**Read this warning before anything else.** Lab 0's README makes a deliberate
case for the VM — class parity *and* **blast radius**. This addendum throws away
both. Use it only if Multipass genuinely will not run on your Mac: a managed or
locked-down laptop that blocks virtualization, too little free disk, or a macOS
version too old for current Multipass.

**Apple Silicon is not a reason** — Multipass runs fine on M1/M2/M3/M4. If you
don't know *why* Multipass failed, bring the exact error to me before falling
back here. The VM is worth fixing.

What you give up by going native:

- **Class parity.** You'll be on Apple's `clang` + `libc++`. The VM — and the
  projector, and the lab graders — run Ubuntu's `gcc`/`clang` + `libstdc++`.
  Warning text differs; a handful of C++20 library features are gated
  differently. Code that builds clean for you can still fail for the grader.
- **Blast radius.** There is no sandbox anymore. Every program you compile and
  run, and every `brew install`, executes as *you* on your real Mac — the same
  account that can read your iCloud, your ssh keys, your browser data. The
  README's L21 security thread still applies; you've just removed the guardrail.
  Be deliberate about what you build and run.
- **A couple of tools.** `valgrind` does not run on Apple Silicon (and is
  unmaintained on recent macOS generally). `gdb` is a code-signing headache on
  macOS. The course leans on AddressSanitizer / UndefinedBehaviorSanitizer
  (`-fsanitize=...`) and `lldb` instead — both work natively — but Lab 3's
  debugger walkthrough is written for the VM's `gdb`.

If you've read all that and still need the fallback:

### 1. Xcode Command Line Tools

```
xcode-select --install
```

Click through the dialog. This gives you `clang++`, `git`, `make`, `lldb`, and
the macOS SDK headers. Verify with `xcode-select -p` — it should print a path.

### 2. Homebrew

If you don't already have it, install Homebrew from <https://brew.sh> (one
command).

### 3. The toolchain

Apple's bundled `clang` lags on C++20 library support, so install Homebrew LLVM
alongside the build tools:

```
brew install llvm cmake ninja
```

`llvm` brings a current `clang++`, `clangd`, `clang-format`, and `clang-tidy`.

**Homebrew LLVM is "keg-only"** — it is not symlinked onto your `PATH` by
default, because macOS already ships its own `clang`. Put Homebrew's ahead of
`/usr/bin`:

```
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc   # Apple Silicon
# echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >> ~/.zshrc    # Intel
exec zsh
```

This is the same step [`admin/editor_setup.md`](../../../admin/editor_setup.md)
describes — there's more detail there.

### 4. Course shell defaults

The VM's cloud-init sets a few conveniences in `.bashrc`. Mirror them in your
Mac's `~/.zshrc` so a bare `make foo` and the `tidy` shortcut behave the same as
they do for everyone on the VM:

```
cat >> ~/.zshrc <<'EOF'
export CXX=clang++
export CXXFLAGS='-std=c++20 -Wall -Wextra -Wpedantic -Wshadow -O0 -g'
tidy() { clang-tidy --quiet "$@" -- -std=c++20 -Wall -Wextra -Wpedantic -Wshadow; }
EOF
exec zsh
```

> One deliberate difference from the VM: the cloud-init sets `CXX=g++` (real GNU
> `gcc`); here it's `CXX=clang++` (Homebrew `clang`), because that's what is
> actually current on a Mac. The lab commands all call `clang++` explicitly
> anyway, so this only affects a bare `make foo` with no Makefile.

### 5. Python venv *(optional — build scripts and the `claude` CLI)*

```
brew install uv
uv venv ~/.venv/ec327
echo 'source ~/.venv/ec327/bin/activate' >> ~/.zshrc
```

### 6. Verify

```
clang++ --version    # should report Homebrew, not Apple
clangd --version     # 17 or higher
git --version
cmake --version
```

`which clang++` should point inside `/opt/homebrew/opt/llvm/bin` (or
`/usr/local/...` on Intel), **not** `/usr/bin`. If it still says `/usr/bin`,
your `PATH` line didn't take effect — close and reopen the terminal.

### 7. Repo and directories — no mount needed

The native path is actually *simpler* here. There's no VM, so there's no mount,
and you skip README sections 2 and 4 entirely — you edit and compile in the
same place.

```
mkdir -p ~/ec327
cd ~/ec327
git clone https://github.com/jbcarruthers/bu-eng-ec327-summer2026.git course
mkdir ~/ec327/work
```

From here, **everything in the README from section 6 onward works as written** —
the copy-out-then-edit discipline, the smoke test, the weekly `git pull`. Just
run the commands in your Mac's Terminal instead of inside `multipass shell
ec327`. The smoke test:

```
cd ~/ec327/course/assignments/labs/lab0_getting_started
clang++ -std=c++20 -Wall -Wextra -Wpedantic -Wshadow hello.cpp -o /tmp/hello
/tmp/hello
```

### Before every submission

Because your environment is not the grader's, build with the exact course flags
and **zero warnings** before you submit — every time. If a classmate or the lab
machine reports an error you can't reproduce on your Mac, that's the parity gap
biting, and the real fix is still to get the VM working. Come talk to me; that
conversation is easier early than late.

---

## Addendum C — Finding your files across WSL and Windows

For Windows / WSL2 users. WSL2 gives you *two* filesystems, and Lab 0 students
lose a lot of time not knowing which one they're looking at.

### The mental model: two filesystems, two views

- **The Linux side** — your Ubuntu home, `/home/<you>/`, written `~`. Fast.
  Real Unix permissions. This is where your course files belong.
- **The Windows side** — your `C:` drive: `C:\Users\<You>\Documents`,
  `\Desktop`, `\Downloads`, and so on.

Each side can see the other, through a bridge:

| You're in… | The other side is at… |
|---|---|
| WSL (Ubuntu shell), reaching Windows | `/mnt/c/...` (also `/mnt/d/`, etc.) |
| Windows (File Explorer), reaching WSL | `\\wsl$\Ubuntu-24.04\home\<you>\` |

### Finding your Windows files from WSL

Windows drives are mounted under `/mnt/`. Start by seeing what's there:

```
ls /mnt
```

You'll see at least `c`. Your Windows user folder is
`/mnt/c/Users/<WindowsUsername>/`.

**You may not know your exact Windows username** — it isn't always what you type
to log in. Three ways to discover it:

```
ls /mnt/c/Users/                       # yours is the folder that isn't Public/Default
cmd.exe /c "echo %USERNAME%"           # ask Windows directly
powershell.exe -NoProfile '$env:USERNAME'
```

(The `cmd.exe` / `powershell.exe` calls may print a harmless
`UNC paths are not supported` notice — ignore it.)

So your Desktop and Documents are *usually*:

```
/mnt/c/Users/<you>/Desktop
/mnt/c/Users/<you>/Documents
```

**The OneDrive trap.** Many Windows installs redirect Documents and Desktop
*into OneDrive*. If the paths above are missing or empty, your real folders are
under OneDrive instead:

```
ls /mnt/c/Users/<you>/                 # is there a "OneDrive" folder?
/mnt/c/Users/<you>/OneDrive/Desktop
/mnt/c/Users/<you>/OneDrive/Documents
```

When you're just not sure, let `find` locate it for you. The `2>/dev/null`
suppresses the permission-denied noise:

```
find /mnt/c/Users/<you> -maxdepth 3 -iname Documents -type d 2>/dev/null
```

Same trick to hunt down a specific file you downloaded on the Windows side:

```
find /mnt/c/Users/<you> -iname "ec327_setup.yaml" 2>/dev/null
```

Copying a file from the Windows side into your course tree then looks like:

```
cp "/mnt/c/Users/<you>/Downloads/something.cpp" ~/ec327/work/
```

### Finding your WSL files from Windows

The other direction. In File Explorer's address bar, type:

```
\\wsl$\Ubuntu-24.04\home\<you>\
```

(Newer Windows also accepts `\\wsl.localhost\Ubuntu-24.04\...`.) WSL also pins a
penguin **Linux** entry in File Explorer's sidebar.

From the Ubuntu shell, you can open the current directory in Windows File
Explorer directly:

```
explorer.exe .
```

Run that from `~/ec327` and the WSL folder pops open in Explorer — a fast way to
confirm you're looking at the right place.

### `wslpath` — translate a path either direction

When you have a path in one form and need the other:

```
wslpath -w ~/ec327                       # Linux → Windows  (\\wsl.localhost\...)
wslpath -u 'C:\Users\Jane\Desktop'       # Windows → Linux  (/mnt/c/Users/Jane/Desktop)
```

Quote Windows paths in single quotes so the shell leaves the backslashes alone.

### Where your course files should actually live: the Linux side

You now know how to reach `/mnt/c/...` — **but don't put `~/ec327` there.** Keep
your course tree on the Linux side (`/home/<you>/ec327`, exactly as the README
says). Three concrete reasons, each of which bites in a graded lab:

- **Permissions.** Files under `/mnt/c` all report mode `777` and the executable
  bit doesn't behave normally. Lab 1 teaches `chmod` and `./program`; those
  lessons simply don't work right on `/mnt/c`.
- **Speed.** Cross-filesystem access goes over a protocol bridge. `git`, `make`,
  and compiles run noticeably slower on `/mnt/c` than on `~`.
- **Line endings.** Files touched by Windows tools pick up CRLF line endings,
  which can break shell scripts with a cryptic `bad interpreter` error.

Use `/mnt/c` as a *doorway* — copy a download in, copy a finished file out — not
as your workshop.

### Let the `claude` CLI map it for you

The `claude` CLI runs natively inside WSL (we use it in class). If you have it
installed, this is exactly the kind of "where on earth are my files" question
it's good at: `cd` to wherever you think your files are, run `claude`, and ask
it to map out your drive layout — it can run `ls /mnt`, `wslpath`, and `find`
for you and explain what it finds. Per the README's "If you get stuck," Lab 0 is
a fine place to lean on AI.

### Discovery toolkit — recap

| Command | What it tells you |
|---|---|
| `ls /mnt` | Which Windows drives are mounted (`c`, `d`, …) |
| `ls /mnt/c/Users/` | The Windows user folders — find yours |
| `cmd.exe /c "echo %USERNAME%"` | Your exact Windows username |
| `find /mnt/c/Users/<you> -iname NAME 2>/dev/null` | Locate a file/folder anywhere on the Windows side |
| `wslpath -w` / `wslpath -u` | Translate a path between Windows and Linux form |
| `explorer.exe .` | Open the current WSL directory in Windows File Explorer |
