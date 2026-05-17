" EC327 .vimrc starter — minimal sensible defaults.
" Vim is here for when you need to edit a file inside the VM in a pinch
" (a quick config tweak, a fast `make` retry). Your primary editor lives
" on your host (Sublime / VS Code).

set nocompatible            " behave like Vim, not vi
syntax on                   " syntax highlighting
filetype plugin indent on   " filetype detection + indent rules

set number                  " line numbers
set ruler                   " cursor position in status line
set showcmd                 " show partial commands
set wildmenu                " tab-completion menu for : commands

set expandtab               " tabs -> spaces
set shiftwidth=4
set tabstop=4
set softtabstop=4
set autoindent
set smartindent

set incsearch               " incremental search
set hlsearch                " highlight matches
set ignorecase smartcase    " case-insensitive unless capital used

set hidden                  " let you switch buffers without saving
set backspace=indent,eol,start
set mouse=a                 " mouse works in most modes
set scrolloff=4             " keep 4 lines visible above/below cursor

set background=dark
" :set background=light  if your terminal is light
