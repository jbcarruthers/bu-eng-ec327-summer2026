#!/usr/bin/env python3
"""
HW3 cross-language interoperability test suite.

This is the deliverable that proves your C++ writers actually produced files a
Python data scientist can load. It builds and runs your program, then loads
data/walks.csv and data/walks.npy the way a real downstream consumer would --
with pandas and numpy -- and checks that:

  * both files load without error,
  * each has the right shape, columns, and dtype,
  * the CSV and the .npy contain the SAME numbers (your two writers agree),
  * the numbers are internally consistent (r_squared == x^2+y^2, etc.),
  * the data looks like real 2D-walk output (mean r^2 ~ N).

Run it from the HW3 directory:

    python3 test_interop.py

Capture its output into test_output.txt for submission:

    python3 test_interop.py 2>&1 | tee test_output.txt

GRACEFUL DEPENDENCIES. The consumer side needs numpy (for .npy) and pandas
(for CSV). If a library is missing, the tests that need it are SKIPPED, not
failed -- but a skipped test earns no credit for that format. Install them
with:

    python3 -m pip install -r requirements.txt

A skip is the suite telling you "I could not check this." Full credit needs
numpy + pandas present and every test PASSING (not skipped).
"""
import shutil
import subprocess
import sys
import unittest
from pathlib import Path

# --- expectations: these MUST match the constants in hw3_main.cpp ----------
EXPECTED_K = 5000
EXPECTED_N = 200
EXPECTED_COLUMNS = ["x", "y", "r_squared", "distance"]

HERE = Path(__file__).resolve().parent
DATA_DIR = HERE / "data"
CSV_PATH = DATA_DIR / "walks.csv"
NPY_PATH = DATA_DIR / "walks.npy"

# --- lazy, graceful imports of the data-science stack ----------------------
try:
    import numpy as np

    HAVE_NUMPY = True
except ImportError:
    HAVE_NUMPY = False

try:
    import pandas as pd

    HAVE_PANDAS = True
except ImportError:
    HAVE_PANDAS = False


def build_and_run():
    """Best-effort: build the C++ program and run it so the data files exist.

    Returns True if data/walks.csv and data/walks.npy both exist afterward.
    Never raises -- a missing toolchain just means the files must already be
    present (the grader runs the binary itself).
    """
    if CSV_PATH.exists() and NPY_PATH.exists():
        return True
    if shutil.which("cmake") is None:
        return CSV_PATH.exists() and NPY_PATH.exists()
    try:
        subprocess.run(["cmake", "-B", "build"], cwd=HERE, check=True,
                       capture_output=True)
        subprocess.run(["cmake", "--build", "build"], cwd=HERE, check=True,
                       capture_output=True)
        exe = HERE / "build" / "hw3_main"
        if exe.exists():
            subprocess.run([str(exe)], cwd=HERE, check=True,
                           capture_output=True)
    except (subprocess.CalledProcessError, OSError):
        pass
    return CSV_PATH.exists() and NPY_PATH.exists()


class TestInterop(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        build_and_run()

    # ---- files exist ------------------------------------------------------
    def test_csv_file_exists(self):
        self.assertTrue(
            CSV_PATH.exists(),
            f"{CSV_PATH} not found -- did write_csv() run and return true?",
        )

    def test_npy_file_exists(self):
        self.assertTrue(
            NPY_PATH.exists(),
            f"{NPY_PATH} not found -- did write_npy() run and return true?",
        )

    # ---- .npy (numpy) -----------------------------------------------------
    @unittest.skipUnless(HAVE_NUMPY, "numpy not installed (pip install numpy)")
    def test_npy_loads_with_shape_and_dtype(self):
        self.assertTrue(NPY_PATH.exists(), "no .npy file to load")
        arr = np.load(NPY_PATH)
        self.assertEqual(arr.ndim, 2, "expected a 2-D array")
        self.assertEqual(arr.shape, (EXPECTED_K, len(EXPECTED_COLUMNS)),
                         f"expected shape ({EXPECTED_K}, 4)")
        self.assertEqual(arr.dtype, np.dtype("<f8"),
                         "expected little-endian float64 ('<f8')")

    # ---- CSV (pandas) -----------------------------------------------------
    @unittest.skipUnless(HAVE_PANDAS, "pandas not installed (pip install pandas)")
    def test_csv_loads_with_columns(self):
        self.assertTrue(CSV_PATH.exists(), "no CSV file to load")
        df = pd.read_csv(CSV_PATH)
        self.assertEqual(list(df.columns), EXPECTED_COLUMNS,
                         f"CSV header must be exactly {EXPECTED_COLUMNS}")
        self.assertEqual(df.shape, (EXPECTED_K, len(EXPECTED_COLUMNS)),
                         f"expected {EXPECTED_K} data rows x 4 columns")

    # ---- the two writers agree -------------------------------------------
    @unittest.skipUnless(HAVE_NUMPY and HAVE_PANDAS,
                         "need numpy AND pandas to cross-check CSV vs .npy")
    def test_csv_matches_npy(self):
        arr = np.load(NPY_PATH)
        df = pd.read_csv(CSV_PATH)
        self.assertTrue(
            np.allclose(df[EXPECTED_COLUMNS].to_numpy(), arr, rtol=0, atol=1e-9),
            "CSV and .npy disagree -- your two writers produced different "
            "numbers (precision loss in the CSV is the usual cause)",
        )

    # ---- the numbers are self-consistent ---------------------------------
    @unittest.skipUnless(HAVE_NUMPY, "numpy not installed")
    def test_internal_consistency(self):
        arr = np.load(NPY_PATH)
        x, y, r2, dist = arr[:, 0], arr[:, 1], arr[:, 2], arr[:, 3]
        self.assertTrue(np.allclose(r2, x * x + y * y),
                        "r_squared column != x^2 + y^2")
        self.assertTrue(np.allclose(dist, np.sqrt(r2)),
                        "distance column != sqrt(r_squared)")

    # ---- it looks like real 2D-walk data ---------------------------------
    @unittest.skipUnless(HAVE_NUMPY, "numpy not installed")
    def test_statistics_plausible(self):
        arr = np.load(NPY_PATH)
        mean_r2 = float(arr[:, 2].mean())
        # For this lattice walk E[r^2] == N exactly; with K=5000 the sample
        # mean lands within a few percent. 15% is a generous, robust band.
        self.assertAlmostEqual(
            mean_r2, EXPECTED_N, delta=0.15 * EXPECTED_N,
            msg=f"mean r^2 = {mean_r2:.1f}, expected ~{EXPECTED_N} "
                f"(is K/N/seed unchanged from the starter main?)",
        )


if __name__ == "__main__":
    if not HAVE_NUMPY or not HAVE_PANDAS:
        missing = [n for n, ok in (("numpy", HAVE_NUMPY), ("pandas", HAVE_PANDAS))
                   if not ok]
        print(f"NOTE: {', '.join(missing)} not installed -- related checks "
              f"will be SKIPPED (no credit for that format).\n"
              f"      python3 -m pip install -r requirements.txt\n",
              file=sys.stderr)
    unittest.main(verbosity=2)
