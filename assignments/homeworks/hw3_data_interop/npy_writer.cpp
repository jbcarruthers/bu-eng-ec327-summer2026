#include "npy_writer.h"

#include <cstdint>
#include <fstream>
#include <string>

#include "DataSet.h"

// =============================================================
//  npy_writer.cpp  --  Part 2. Implement write_npy().
//
//  Sketch (read npy_writer.h for the full format, and the handout Hints):
//
//    1. Open an std::ofstream in BINARY mode:
//         std::ofstream out(path, std::ios::binary);
//         if (!out) return false;
//
//    2. Build the header dict string for shape (K, 4), e.g. with a
//       std::ostringstream:
//         {'descr': '<f8', 'fortran_order': False, 'shape': (K, 4), }
//
//    3. Pad it: the magic(6) + version(2) + len(2) + dict must total a
//       multiple of 64, and the dict must end in '\n'. So append spaces
//       until  (10 + dict.size() + 1)  is a multiple of 64, then append
//       '\n'. HEADER_LEN is the size of that padded, newline-terminated
//       string.
//
//    4. Write, in order:
//         - the 6 magic bytes: 0x93 then "NUMPY"
//         - the 2 version bytes: 0x01, 0x00
//         - HEADER_LEN as a little-endian uint16 (write the low byte, then
//           the high byte -- or memcpy a uint16_t on a little-endian box)
//         - the padded header string
//
//    5. Write the data: for each row, write the four doubles in column
//       order, each as raw bytes:
//         out.write(reinterpret_cast<const char*>(&value), sizeof(double));
//
//    6. return true;
//
//  Verify your file the moment you can write one:  python3 -c "import numpy,
//  sys; a = numpy.load(sys.argv[1]); print(a.shape, a.dtype)" data/walks.npy
//  -- you want  (K, 4) float64.
// =============================================================

bool write_npy([[maybe_unused]] const std::string& path,
               [[maybe_unused]] const DataSet& data) {
  // Part 2 -- implement me.
  return false;
}
