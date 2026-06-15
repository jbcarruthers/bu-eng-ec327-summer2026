#!/usr/bin/env python3
"""
HW3 smoke test -- necessary but not sufficient.

Checks the C++ side: that your project builds, runs, and produces the two
output files. It does NOT load or validate the data -- that is what the
cross-language test_interop.py does (and that is the graded deliverable). Run
this first to confirm the build works, then run test_interop.py.

    python3 smoke_test.py

Exit 0 means the build/run plumbing is fine. Non-zero means fix the build
before worrying about the data.
"""
import subprocess
import sys
from pathlib import Path

GREEN, RED, YELLOW, RESET = "\033[32m", "\033[31m", "\033[33m", "\033[0m"


def ok(msg):
    print(f"  {GREEN}OK{RESET}    {msg}")


def bad(msg):
    print(f"  {RED}FAIL{RESET}  {msg}")


def note(msg):
    print(f"  {YELLOW}note{RESET}  {msg}")


HERE = Path(__file__).resolve().parent


def check_files():
    print("\nChecking required source files...")
    required = [
        "hw3_main.cpp", "DataSet.h", "DataSet.cpp",
        "csv_writer.h", "csv_writer.cpp",
        "npy_writer.h", "npy_writer.cpp",
        "Walker2D.h", "Walker2D.cpp", "CMakeLists.txt",
    ]
    final = ["test_output.txt", "reflection.md", "ai_disclosure.md"]
    failures = 0
    for f in required:
        if (HERE / f).exists():
            ok(f)
        else:
            bad(f"{f} is missing")
            failures += 1
    for f in final:
        if (HERE / f).exists():
            ok(f)
        else:
            note(f"{f} not present yet (needed in your final submission)")
    return failures


def build():
    print("\nBuilding with cmake...")
    try:
        subprocess.run(["cmake", "-B", "build"], cwd=HERE, check=True,
                       capture_output=True, text=True)
        subprocess.run(["cmake", "--build", "build"], cwd=HERE, check=True,
                       capture_output=True, text=True)
    except FileNotFoundError:
        bad("cmake not found on PATH -- is your toolchain set up (Lab 0)?")
        return 1
    except subprocess.CalledProcessError as e:
        bad("build failed:")
        print(e.stdout or "", e.stderr or "", sep="\n")
        return 1
    ok("cmake build succeeded")
    return 0


def run_and_check_outputs():
    print("\nRunning ./build/hw3_main and checking it writes the files...")
    exe = HERE / "build" / "hw3_main"
    if not exe.exists():
        bad("build/hw3_main not found -- did the build produce it?")
        return 1
    try:
        proc = subprocess.run([str(exe)], cwd=HERE, check=True,
                              capture_output=True, text=True, timeout=60)
    except subprocess.CalledProcessError:
        bad("hw3_main exited non-zero")
        return 1
    except subprocess.TimeoutExpired:
        bad("hw3_main did not finish within 60s")
        return 1
    ok("hw3_main ran and exited cleanly")
    print(proc.stdout.rstrip())

    failures = 0
    for rel in ("data/walks.csv", "data/walks.npy"):
        p = HERE / rel
        if p.exists() and p.stat().st_size > 0:
            ok(f"{rel} written ({p.stat().st_size} bytes)")
        else:
            bad(f"{rel} not written -- is your writer implemented and called "
                f"in hw3_main.cpp, and did it return true?")
            failures += 1
    return failures


def main():
    print("=" * 60)
    print("HW3 smoke test -- build, run, files produced")
    print("=" * 60)
    failures = check_files()
    if failures:
        print(f"\n{RED}{failures} required file(s) missing. Stop here.{RESET}")
        return 1
    if build():
        return 1
    failures = run_and_check_outputs()
    print()
    if failures:
        print(f"{RED}Smoke test FAILED ({failures} problem(s)).{RESET} "
              f"Implement the writers, then re-run.")
        return 1
    print(f"{GREEN}Smoke test passed.{RESET} Now run:  python3 test_interop.py")
    print("Remember: passing the smoke test is necessary, not sufficient.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
