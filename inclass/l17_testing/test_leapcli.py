"""EC327 L17 -- test_leapcli.py

pytest as an ALTERNATIVE test harness for a C++ program.

We are not testing Python here. We compile the C++ CLI once, then drive the
*binary* from Python with subprocess and assert on its stdout and exit code.
This is black-box / integration testing: we never see is_leap_year's source,
only the program's observable behavior -- exactly what a user (or another
program in a pipeline) sees.

Why reach for pytest instead of doctest/Catch2?
  - The thing under test is a whole program with a CLI contract (args, stdout,
    stderr, exit code), not a single function you can #include.
  - You want to test across a language boundary, or glue together programs
    written in different languages.
  - pytest's @parametrize makes a table of cases trivial, and its failure
    reporting is excellent with zero boilerplate.

Run it:
    g++ -std=c++20 -Wall -Wextra leapcli.cpp -o leapcli
    pytest -v test_leapcli.py
"""

import subprocess
from pathlib import Path

import pytest

CLI = Path(__file__).parent / "leapcli"


def run(*args):
    """Run the compiled CLI and return (exit_code, stdout, stderr)."""
    result = subprocess.run(
        [str(CLI), *args],
        capture_output=True,
        text=True,
        timeout=5,
    )
    return result.returncode, result.stdout.strip(), result.stderr.strip()


@pytest.fixture(scope="session", autouse=True)
def _require_binary():
    """Fail loudly with a build hint if the student forgot to compile."""
    if not CLI.exists():
        pytest.fail(
            f"{CLI.name} not built. Run:\n"
            f"  g++ -std=c++20 -Wall -Wextra leapcli.cpp -o leapcli"
        )


# A table of cases -- the same spec as the C++ unit tests, expressed as data.
@pytest.mark.parametrize(
    "year, expect_leap",
    [
        ("2024", True),   # ordinary leap
        ("2023", False),  # ordinary non-leap
        ("1900", False),  # century trap
        ("2000", True),   # 400-divisible trap's trap
        ("2100", False),  # century, not 400
        ("0", True),      # boundary
    ],
)
def test_leap_classification(year, expect_leap):
    code, out, _ = run(year)
    if expect_leap:
        assert out == f"{year} is a leap year"
        assert code == 0   # exit code is part of the contract
    else:
        assert out == f"{year} is not a leap year"
        assert code == 1


def test_non_integer_is_a_usage_error():
    code, out, err = run("hello")
    assert code == 2          # distinct exit code for bad input
    assert out == ""           # nothing on stdout
    assert "not an integer" in err


def test_missing_argument_prints_usage():
    code, _, err = run()
    assert code == 2
    assert "usage" in err.lower()
