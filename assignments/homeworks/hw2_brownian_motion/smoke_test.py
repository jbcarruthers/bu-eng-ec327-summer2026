#!/usr/bin/env python3
"""
HW2 smoke test — necessary but not sufficient.

Verifies your submission builds, runs, and produces output that looks like a
working 2D Brownian-motion simulation. The instructor will read your code and
run additional cases during grading.

Run from the HW2 directory after writing your code:

    python3 smoke_test.py

Exit code 0 means all checks passed. Non-zero means something is wrong; read
the message and fix it before submitting.

This script is your first line of defense, the way CI is for a real
engineering team. Run it. Pass it. Then submit.
"""
import re
import subprocess
import sys
from pathlib import Path

GREEN = "\033[32m"
RED = "\033[31m"
YELLOW = "\033[33m"
RESET = "\033[0m"


def ok(msg):
    print(f"  {GREEN}OK{RESET}    {msg}")


def bad(msg):
    print(f"  {RED}FAIL{RESET}  {msg}")


def note(msg):
    print(f"  {YELLOW}note{RESET}  {msg}")


def check_files():
    print("\nChecking required source files...")
    required = ["Walker2D.h", "Walker2D.cpp", "hw2_main.cpp", "CMakeLists.txt"]
    later_parts = ["Histogram2D.h", "Histogram2D.cpp"]
    final_submission = ["output.txt", "reflection.md", "ai_disclosure.md"]

    failures = 0
    for f in required:
        if Path(f).exists():
            ok(f)
        else:
            bad(f"{f} is missing")
            failures += 1
    for f in later_parts:
        if Path(f).exists():
            ok(f)
        else:
            note(f"{f} not present yet (needed for Part 4)")
    for f in final_submission:
        if Path(f).exists():
            ok(f)
        else:
            note(f"{f} not present yet (needed for final submission)")
    return failures == 0


def check_build():
    print("\nBuilding with CMake...")
    cfg = subprocess.run(
        ["cmake", "-B", "build", "-S", "."],
        capture_output=True,
        text=True,
    )
    if cfg.returncode != 0:
        bad("cmake configure failed:")
        print(cfg.stderr)
        return False
    ok("cmake configure succeeded")

    build = subprocess.run(
        ["cmake", "--build", "build"],
        capture_output=True,
        text=True,
    )
    if build.returncode != 0:
        bad("compilation failed:")
        print(build.stderr)
        return False
    ok("compilation succeeded")
    return True


def check_runs_and_output():
    print("\nRunning ./build/hw2_main...")
    binary = Path("build/hw2_main")
    if not binary.exists():
        bad("build/hw2_main not found after build")
        return False

    try:
        result = subprocess.run(
            [str(binary)],
            capture_output=True,
            text=True,
            timeout=60,
        )
    except subprocess.TimeoutExpired:
        bad("binary timed out after 60 seconds (infinite loop?)")
        return False

    if result.returncode != 0:
        bad(f"binary exited with code {result.returncode}")
        if result.stderr:
            print(result.stderr)
        return False
    ok(f"binary ran (exit code 0, {len(result.stdout)} bytes of output)")

    return inspect_output(result.stdout)


def inspect_output(out):
    print("\nInspecting output...")
    failures = 0

    # Pull every "<r^2> = X" (mean-squared displacement) value. Accept a few
    # spellings students might use: <r^2>, r^2, msd, mean-squared.
    msd = re.findall(
        r"(?:<\s*r\s*\^?\s*2\s*>|r\^2|msd|mean[ -]?squared[^=]*)\s*=\s*"
        r"(-?\d+\.?\d*)",
        out,
        flags=re.IGNORECASE,
    )

    if not msd:
        bad(
            "no mean-squared-displacement line found "
            "(expected something like '<r^2> = 100.4')"
        )
        failures += 1
    else:
        # Use the LAST reported value — typically the largest-K run at N=100,
        # where the estimate is sharpest. For this lattice walk the TRUE
        # mean-squared displacement equals N, so a run near N=100 should land
        # in a generous band around 100. We allow a wide range because the
        # student may report a different N as their last line.
        v = float(msd[-1])
        ok(f"last mean-squared displacement: {v}")
        if v < 2:
            bad(
                f"<r^2> = {v} is implausibly small — walkers not moving, "
                "or r_squared() returning 0?"
            )
            failures += 1
        elif v > 5000:
            bad(
                f"<r^2> = {v} is implausibly large — overflow, or a very "
                "large N? (for N=100 it should be near 100)"
            )
            failures += 1
        else:
            ok("mean-squared displacement is in a plausible range")

    # Histogram check — HW2 prints TWO histograms (x-coordinate + distance),
    # so expect a healthy number of bar lines.
    star_lines = [ln for ln in out.split("\n") if "*" in ln]
    if len(star_lines) < 10:
        bad(
            f"only {len(star_lines)} lines contain '*' — did you print BOTH "
            "histograms (x-coordinate AND distance from origin)?"
        )
        failures += 1
    else:
        ok(f"histograms have {len(star_lines)} bar lines total")

    return failures == 0


def main():
    print("=" * 60)
    print("  HW2 smoke test")
    print("=" * 60)
    print("Passing is necessary but not sufficient.")
    print("This script does not grade your work — it catches gross failures.")
    print("The instructor reads your code and runs additional cases.")

    if not check_files():
        print(f"\n{RED}FAILED — required files missing.{RESET}")
        sys.exit(1)

    if not check_build():
        print(f"\n{RED}FAILED — build broken.{RESET}")
        sys.exit(1)

    if not check_runs_and_output():
        print(f"\n{RED}FAILED — output does not look right.{RESET}")
        sys.exit(1)

    print(f"\n{GREEN}All smoke checks passed.{RESET}")
    print("Reminder: this is a smoke test, not a grader.")
    print("It does NOT check that Part 3 uses the STL, that your class keeps")
    print("its data private, or that your reflection answers the prompts.")
    sys.exit(0)


if __name__ == "__main__":
    main()
