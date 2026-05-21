#!/usr/bin/env python3
"""
HW1 smoke test — necessary but not sufficient.

Verifies your submission builds, runs, and produces output that looks like
a working random walker. The instructor will read your code and run
additional cases during grading.

Run from the HW1 directory after writing your code:

    python3 smoke_test.py

Exit code 0 means all checks passed. Non-zero means something is wrong;
read the message and fix it before submitting.

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
    required = ["Walker.h", "Walker.cpp", "hw1_main.cpp", "CMakeLists.txt"]
    later_parts = ["Histogram.h", "Histogram.cpp"]
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
    print("\nRunning ./build/hw1_main...")
    binary = Path("build/hw1_main")
    if not binary.exists():
        bad("build/hw1_main not found after build")
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

    # Pull all numeric "mean = X" and "variance = Y" pairs.
    means = re.findall(r"mean\s*=\s*(-?\d+\.?\d*)", out)
    vars_ = re.findall(r"variance\s*=\s*(-?\d+\.?\d*)", out)

    if not means:
        bad("no 'mean = ...' line found in output")
        failures += 1
    if not vars_:
        bad("no 'variance = ...' line found in output")
        failures += 1

    if means and vars_:
        # Use the LAST pair — typically the largest-K run, where the
        # estimates are sharpest and a correct submission will have
        # mean very close to zero.
        m, v = float(means[-1]), float(vars_[-1])
        ok(f"last reported pair: mean={m}, variance={v}")

        # Mean should be near zero. For K=10000, N=100 the standard
        # error of the mean is ~0.1, so |mean| > 2 is essentially
        # impossible for correct code. We give a generous bound here.
        if abs(m) > 2.0:
            bad(
                f"mean={m} is far from zero — RNG bias, "
                "off-by-one, or step-direction bug?"
            )
            failures += 1
        else:
            ok("mean is plausibly near zero")

        # Variance should be in a reasonable range. Anything between 5
        # and 10000 is fine for the various (K, N) combos students
        # might pick.
        if v < 5:
            bad(
                f"variance={v} is suspiciously small — "
                "accumulator wrong, or wrong N?"
            )
            failures += 1
        elif v > 10000:
            bad(
                f"variance={v} is suspiciously large — "
                "overflow or off-by-one in N?"
            )
            failures += 1
        else:
            ok("variance is in a plausible range")

    # Histogram check — look for the bar chart.
    star_lines = [ln for ln in out.split("\n") if "*" in ln]
    if len(star_lines) < 5:
        bad(
            f"only {len(star_lines)} lines contain '*' — "
            "did you print the histogram?"
        )
        failures += 1
    else:
        ok(f"histogram has {len(star_lines)} bar lines")

    return failures == 0


def main():
    print("=" * 60)
    print("  HW1 smoke test")
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
    print("Read your spec carefully and run additional (K, N) cases yourself.")
    sys.exit(0)


if __name__ == "__main__":
    main()
