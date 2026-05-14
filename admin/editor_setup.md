# EC327 editor setup — clangd + clang-format

The course ships **three tool configs** at `ec327/`:

| File | What it does |
|---|---|
| `.clangd` | Language server — real-time error squiggles, type inlays, jump-to-definition, autocomplete |
| `.clang-tidy` | Static analyzer — style + bug checks. Run by clangd automatically. |
| `.clang-format` | Mechanical layout (indentation, braces, line breaks) |

All three are picked up automatically by any editor that talks to clangd — as long as the editor has the LSP plumbing installed. They live at the `ec327/` root and apply to every C++ file under it (clangd walks up the tree).

---

## Prerequisites — install the tools

### macOS

Apple's bundled `clangd` (from Xcode CLT, at `/usr/bin/clangd`) is usually one major version behind on C++20 library support. **Install Homebrew LLVM:**

```
brew install llvm
```

**Important: Homebrew LLVM is "keg-only"** — it does *not* get symlinked into `/opt/homebrew/bin` by default, because macOS already provides `clang`/`clang++`/`clangd`. To use the Homebrew version, you must add it to `$PATH` *ahead of* `/usr/bin`. The brew install prints the exact command at the end — paste this one-liner:

```
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc   # Apple Silicon
# echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >> ~/.zshrc    # Intel
exec zsh   # or close + reopen your terminal
```

Verify: `which clangd` should return `/opt/homebrew/opt/llvm/bin/clangd` (not `/usr/bin/clangd`), and `clangd --version` should report **17 or higher**.

**Prerequisite — Xcode Command Line Tools.** Homebrew clang++ needs the macOS SDK headers, which come from CLT. If you have Xcode installed you already have them. Otherwise: `xcode-select --install`. To verify: `xcode-select -p` should print a path.

**On older macOS** (13.x / 14.x), libc++ may mark some C++20 library features (`std::format`, parts of `std::ranges`) as "unavailable" via deployment-target gates even though the library actually ships them. The course `.clangd` defensively defines `_LIBCPP_DISABLE_AVAILABILITY` to bypass these gates, so this should be invisible to you. If you nonetheless see "function unavailable on macOS X.Y" errors, check that your editor really picked up the project `.clangd` (restart it).

### Ubuntu / WSL2

```
sudo apt install clangd-17 clang-format clang-tidy
```

(or whatever the current major version is — 17+ recommended for C++20 library support.)

---

## Per-editor setup

Pick one. Any editor with LSP support works — the same `.clangd` / `.clang-tidy` / `.clang-format` files drive them all. **If you don't already have a preferred editor, install VSCode** (free, runs identically on Mac and Windows, has the smoothest clangd setup, and the WSL extension is the single biggest UX win for Windows users). I use Sublime Text in lecture but you don't need to match.

### VSCode (recommended default)

1. **Install VSCode** from <https://code.visualstudio.com>.
2. **Install the `clangd` extension by LLVM** (Extensions panel → search "clangd" → install the one published by `LLVM`).
3. **Disable Microsoft's C/C++ extension's IntelliSense** if you have it installed — it fights clangd. Open the C/C++ extension's settings and set `C_Cpp.intelliSenseEngine` to `disabled`. (You can keep the extension for its debugger.)
4. **Enable format-on-save**: settings → search "Format On Save" → tick the box.
5. **Windows-WSL2 users — install the `WSL` extension** (also from Microsoft). Once it's installed, open the WSL command palette (`Ctrl+Shift+P` → "WSL: Connect to WSL") and open your `~/ec327/` folder *inside* WSL. Files, terminals, tools — everything runs Linux-side, transparently. This avoids every "path looks weird on Windows" issue you'd otherwise hit.
6. **Mac users on Homebrew LLVM**: VSCode's clangd extension auto-detects the binary on `$PATH`. If you launched VSCode from Spotlight rather than from a terminal, your `~/.zshrc` PATH changes may not be inherited — launch VSCode from a terminal once (`code .`), or set the extension setting `clangd.path` to `/opt/homebrew/opt/llvm/bin/clangd`.

**Test it**: open `ec327/inclass/datastructures.cpp`. Expected: no red squiggles, hover over `std::format` shows its signature, save normalizes whitespace silently.

### Sublime Text

1. Install **Package Control**: <https://packagecontrol.io/installation>
2. `Cmd-Shift-P` → `Install Package` → **LSP**
3. `Cmd-Shift-P` → `Install Package` → **LSP-clangd**
4. LSP-clangd auto-detects `clangd` on `$PATH`. No further config needed if Homebrew LLVM is on your PATH.
5. For format-on-save, add `"lsp_format_on_save": true` to your LSP user settings (Preferences → Package Settings → LSP → Settings).
6. **Mac**: launch Sublime from a terminal once (`subl .`) so it inherits the Homebrew-LLVM PATH from your shell. Otherwise it may use Apple's older `clangd`. See troubleshooting below.

**Test it**: open `ec327/inclass/datastructures.cpp`. Same expected behavior as VSCode.

### CLion / Qt Creator

Built-in support for both clangd and clang-format. Open the `ec327/` directory; the configs are picked up automatically. Free for students with a `.edu` email via the JetBrains student program.

### Vim / Neovim

`coc-clangd`, or built-in LSP via `nvim-lspconfig` + `lspconfig.clangd.setup{}`. Format via `vim-clang-format` or `:lua vim.lsp.buf.format()`.

### Emacs

`lsp-mode` + auto-detected `clangd`. Format via `clang-format.el`.

---

## Troubleshooting

**Still seeing red squiggles?**

1. Run `which clangd` *and* `clangd --version` from a terminal. If `which` returns `/usr/bin/clangd`, you're still on Apple's keg-only-shadowed version — the `export PATH=...` line wasn't added to `~/.zshrc`, or your editor was launched from a shell that didn't have the updated PATH. (GUI editors launched from Spotlight inherit a *different* environment than ones launched from a terminal. Safest fix: launch your editor from a terminal once after the PATH fix — `code .` for VSCode, `subl .` for Sublime — so it inherits the updated `$PATH`.) Want the Homebrew one: `/opt/homebrew/opt/llvm/bin/clangd`, version 17+.
2. Make sure the file is *inside* the `ec327/` tree. clangd walks up looking for `.clangd`; outside the tree, it falls back to defaults.
3. Restart the editor — clangd caches per-session.
4. For multi-file labs (Lab 2+), generate a `compile_commands.json` via `bear -- make` or CMake. Single-file demos in `inclass/` / `lab0` / `lab1` don't need one — the `.clangd` `CompileFlags` cover them.

**Diagnostics in editor don't match build warnings?**

The `.clangd` advertises `-std=c++20 -Wall -Wextra -Wpedantic -Wshadow`. Make sure the lab Makefile uses the same set, so editor and build agree.

---

## Target Makefile baseline

When labs ship with Makefiles, the flag set will be:

```make
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -g
SANFLAGS = -fsanitize=address,undefined        # Debug build adds these
```

Same `-std` and warning set as `.clangd` → editor squiggles match the build.
