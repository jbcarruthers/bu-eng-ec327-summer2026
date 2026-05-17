# EC327 .bashrc starter
# Append this to your existing ~/.bashrc, or replace it.

# --- Prompt -------------------------------------------------------------
# (ec327) user@host:cwd$
export PS1='\[\e[36m\](ec327)\[\e[0m\] \u@\h:\[\e[33m\]\w\[\e[0m\]\$ '

# --- History hygiene ----------------------------------------------------
HISTSIZE=10000
HISTFILESIZE=20000
HISTCONTROL=ignoredups:erasedups
shopt -s histappend

# --- Editors ------------------------------------------------------------
export EDITOR=vim
export VISUAL=vim

# --- Aliases ------------------------------------------------------------
alias ll='ls -lah --color=auto'
alias la='ls -A --color=auto'
alias l='ls -CF --color=auto'
alias grep='grep --color=auto'
alias ..='cd ..'
alias ...='cd ../..'

# --- C++ build defaults -------------------------------------------------
# With these set, `make foo` compiles foo.cpp into ./foo using make's
# built-in pattern rule — no Makefile needed for single-file programs.
# Make also checks timestamps, so a second `make foo` is a fast no-op if
# nothing changed. (For multi-file projects, write a real Makefile or use
# CMake — both are taught in this course.)
export CXX=g++
export CXXFLAGS="-std=c++20 -Wall -Wextra -O0 -g"

# --- Auto-activate Python venv -----------------------------------------
# (cloud-init may have already added this; keep one copy.)
if [ -f "$HOME/.venv/ec327/bin/activate" ]; then
    source "$HOME/.venv/ec327/bin/activate"
fi
