#!/usr/bin/env python3
"""ec327_zip.py — build and check your EC327 submission .zip.

EC327 submissions are a single .zip uploaded at curl.bu.edu/ec327. This
helper bundles your deliverables into that zip and sanity-checks it the same
way the course website does, so "I forgot a file" never costs you points.

It is plain Python 3 (>= 3.8), standard library only — no install, no pip,
nothing to set up. Run it from the directory that holds your files.

  # See what an assignment expects:
  python3 ec327_zip.py list
  python3 ec327_zip.py list hw1

  # Build the submission zip from files in the current directory:
  python3 ec327_zip.py build lab1
  python3 ec327_zip.py build hw1 -o my_hw1.zip extra_notes.md

  # Check a zip you already made (yours or this tool's):
  python3 ec327_zip.py check hw1
  python3 ec327_zip.py check hw1 SomeName.zip

Exit status is 0 when every required file is present, 1 otherwise — handy if
you want to gate a script on it.

The check mirrors the website exactly: comparison is case-insensitive,
folders inside the zip are ignored (so zipping a folder is fine), and macOS
'__MACOSX' / '.DS_Store' cruft is skipped.
"""

import argparse
import sys
import zipfile
from pathlib import Path, PurePosixPath

# Required-file manifests. These match the website's per-assignment
# "required_files" list — the same set the upload page checks inside your zip.
# Names are matched case-insensitively. Files you include that aren't on the
# list are fine; they're reported as "also included", not errors.
MANIFESTS = {
    "lab1": [
        "lab1_transcript.txt",
        "poem.cpp",
        "infinite.cpp",
        "CMakeLists.txt",
        "lab1_ai_disclosure.md",
    ],
    "hw1": [
        "hw1_main.cpp",
        "Walker.h",
        "Walker.cpp",
        "Histogram.h",
        "Histogram.cpp",
        "CMakeLists.txt",
    ],
}

_ZIP_JUNK_PREFIXES = ("__MACOSX/",)
_ZIP_JUNK_BASENAMES = {".DS_Store", "Thumbs.db"}

# Never bundle these into a submission — they're build artifacts, not source.
_SKIP_DIRS = {"build", ".git", "__pycache__", ".vscode", ".idea"}
_SKIP_EXTS = {".o", ".obj", ".out", ".exe", ".bin"}


def _die(msg: str) -> "int":
    print(f"error: {msg}", file=sys.stderr)
    return 2


def _known_assignment(name: str) -> bool:
    return name in MANIFESTS


def _zip_member_basenames(zip_path: Path) -> list[str]:
    """De-duplicated leaf names of the real file members of a zip — the same
    rule the website uses."""
    out: list[str] = []
    seen: set[str] = set()
    with zipfile.ZipFile(zip_path) as zf:
        for info in zf.infolist():
            if info.is_dir():
                continue
            name = info.filename
            if any(name.startswith(p) for p in _ZIP_JUNK_PREFIXES):
                continue
            base = PurePosixPath(name).name
            if not base or base in _ZIP_JUNK_BASENAMES:
                continue
            key = base.lower()
            if key in seen:
                continue
            seen.add(key)
            out.append(base)
    return out


def _report(required: list[str], present_names: list[str]) -> bool:
    """Print the present/missing/extra breakdown. Return True if complete."""
    have = {n.lower() for n in present_names}
    req_lower = {r.lower() for r in required}
    missing = [r for r in required if r.lower() not in have]
    extra = [n for n in present_names if n.lower() not in req_lower]

    print(f"  required: {len(required) - len(missing)}/{len(required)} present")
    for r in required:
        mark = "OK " if r.lower() in have else "MISS"
        print(f"    [{mark}] {r}")
    if extra:
        print("  also included (not required, fine to keep):")
        for n in sorted(extra):
            print(f"    [+]   {n}")
    if missing:
        print()
        print(f"  MISSING {len(missing)} required file(s): "
              + ", ".join(missing))
        print("  Add them and rebuild before you submit.")
    else:
        print()
        print("  All required files present. Good to upload.")
    return not missing


def _gather_local(required: list[str], extras: list[str]) -> list[Path]:
    """Find files in the current directory matching the manifest (case-
    insensitively) plus any explicit extra paths. Skips build artifacts."""
    cwd = Path.cwd()
    by_lower: dict[str, Path] = {}
    for p in sorted(cwd.iterdir()):
        if p.is_dir():
            continue
        if p.suffix.lower() in _SKIP_EXTS:
            continue
        by_lower.setdefault(p.name.lower(), p)

    chosen: list[Path] = []
    seen: set[str] = set()

    def add(path: Path) -> None:
        key = path.name.lower()
        if key not in seen:
            seen.add(key)
            chosen.append(path)

    for req in required:
        hit = by_lower.get(req.lower())
        if hit is not None:
            add(hit)
    # Auto-include common companion files if present (handouts often ask for
    # these even when they aren't on the strict required list).
    for companion in ("output.txt", "reflection.md", "ai_disclosure.md"):
        hit = by_lower.get(companion)
        if hit is not None:
            add(hit)
    for raw in extras:
        p = Path(raw)
        if not p.is_file():
            print(f"  warning: extra file not found, skipping: {raw}",
                  file=sys.stderr)
            continue
        add(p)
    return chosen


def cmd_list(args: argparse.Namespace) -> int:
    names = [args.assignment] if args.assignment else sorted(MANIFESTS)
    for name in names:
        if not _known_assignment(name):
            return _die(f"unknown assignment {name!r}; "
                        f"known: {', '.join(sorted(MANIFESTS))}")
        print(f"{name}: required files ({len(MANIFESTS[name])})")
        for f in MANIFESTS[name]:
            print(f"  - {f}")
    return 0


def cmd_build(args: argparse.Namespace) -> int:
    if not _known_assignment(args.assignment):
        return _die(f"unknown assignment {args.assignment!r}; "
                    f"known: {', '.join(sorted(MANIFESTS))}")
    required = MANIFESTS[args.assignment]
    out = Path(args.output or f"{args.assignment}_submission.zip")
    files = _gather_local(required, args.extra)
    if not files:
        return _die("no matching files found in the current directory. "
                    "cd to the folder that holds your deliverables.")

    if out.exists():
        out.unlink()
    with zipfile.ZipFile(out, "w", zipfile.ZIP_DEFLATED) as zf:
        for p in files:
            zf.write(p, arcname=p.name)
    print(f"Wrote {out} ({out.stat().st_size} bytes, {len(files)} files).")
    print()
    print(f"Checking {out} against the {args.assignment} manifest:")
    ok = _report(required, _zip_member_basenames(out))
    print()
    print(f"Now upload {out} at curl.bu.edu/ec327 "
          f"(one .zip, nothing else).")
    return 0 if ok else 1


def _default_zip(assignment: str) -> Path | None:
    """Pick a zip to check: <assignment>_submission.zip if present, else the
    only .zip in the directory, else None (ambiguous)."""
    pref = Path(f"{assignment}_submission.zip")
    if pref.is_file():
        return pref
    zips = sorted(Path.cwd().glob("*.zip"))
    if len(zips) == 1:
        return zips[0]
    return None


def cmd_check(args: argparse.Namespace) -> int:
    if not _known_assignment(args.assignment):
        return _die(f"unknown assignment {args.assignment!r}; "
                    f"known: {', '.join(sorted(MANIFESTS))}")
    required = MANIFESTS[args.assignment]
    if args.zipfile:
        zpath = Path(args.zipfile)
    else:
        zpath = _default_zip(args.assignment)
        if zpath is None:
            return _die("couldn't pick a zip to check. Pass one explicitly: "
                        f"python3 ec327_zip.py check {args.assignment} "
                        "YOURFILE.zip")
    if not zpath.is_file():
        return _die(f"no such file: {zpath}")
    if not zipfile.is_zipfile(zpath):
        return _die(f"{zpath} is not a valid .zip archive.")
    print(f"Checking {zpath} against the {args.assignment} manifest:")
    ok = _report(required, _zip_member_basenames(zpath))
    return 0 if ok else 1


def main(argv: "list[str] | None" = None) -> int:
    parser = argparse.ArgumentParser(
        prog="ec327_zip.py",
        description="Build and check your EC327 submission .zip.",
    )
    sub = parser.add_subparsers(dest="command", required=True)

    p_list = sub.add_parser("list", help="show an assignment's required files")
    p_list.add_argument("assignment", nargs="?",
                        help="lab1, hw1, ... (omit to list all)")
    p_list.set_defaults(func=cmd_list)

    p_build = sub.add_parser("build", help="zip up deliverables in this dir")
    p_build.add_argument("assignment", help="lab1, hw1, ...")
    p_build.add_argument("-o", "--output", help="output zip name")
    p_build.add_argument("extra", nargs="*",
                        help="extra files to include beyond the manifest")
    p_build.set_defaults(func=cmd_build)

    p_check = sub.add_parser("check", help="verify a zip against the manifest")
    p_check.add_argument("assignment", help="lab1, hw1, ...")
    p_check.add_argument("zipfile", nargs="?",
                        help="zip to check (default: auto-detect)")
    p_check.set_defaults(func=cmd_check)

    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
