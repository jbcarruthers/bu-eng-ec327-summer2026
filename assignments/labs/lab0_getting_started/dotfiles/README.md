# EC327 dotfiles starter

Sensible defaults for the development VM (Multipass) or WSL2 Ubuntu shell.
None of this is required — adopt what you like, ignore what you don't.

## What's here

| File         | What it sets up                                              |
|--------------|--------------------------------------------------------------|
| `.bashrc`    | Friendly prompt, useful aliases, history hygiene             |
| `.vimrc`     | Syntax highlighting, line numbers, sane indentation          |
| `.gitconfig` | Default branch `main`, push autosetup, color, placeholders for name/email |

## How to use

From inside the VM (or WSL2 shell), with the host work directory mounted at
`/home/ubuntu/work` (Multipass) or your preferred path on WSL2:

```bash
cp /home/ubuntu/work/dotfiles/.bashrc    ~/.bashrc
cp /home/ubuntu/work/dotfiles/.vimrc     ~/.vimrc
cp /home/ubuntu/work/dotfiles/.gitconfig ~/.gitconfig
```

Then **edit `~/.gitconfig`** and fill in your name and email. Git will not let
you commit until it knows who you are.

```bash
git config --global user.name "Your Name"
git config --global user.email "you@bu.edu"
```

(That writes back to `~/.gitconfig` — either method works.)

Open a fresh shell to pick up the new `.bashrc`.
