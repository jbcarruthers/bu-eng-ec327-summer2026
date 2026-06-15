#pragma once

#include <string>

#include "DataSet.h"

// =============================================================
//  npy_writer.h  --  EC327 Summer 2026, Homework 3, Part 2
//
//  Part 1 wrote text. Now you write BINARY: the actual NumPy .npy format,
//  the file that numpy.load() reads. CSV is human-readable but bloated and
//  lossy-by-default (you had to fight the precision). A .npy file stores the
//  raw IEEE-754 bytes -- smaller, exact, and instant to load. The cost is
//  that you have to lay the bytes out exactly right. That is the lesson:
//  what a binary format buys you, and what it costs to write one by hand.
//
//  You will write the table as a single 2-D array of shape (K, 4), dtype
//  float64, C-order (row-major). The .npy v1.0 layout is:
//
//    1. magic string         "\x93NUMPY"            (6 bytes)
//    2. version              0x01 0x00              (2 bytes: major, minor)
//    3. HEADER_LEN           little-endian uint16   (2 bytes)
//    4. header dict (ASCII)  HEADER_LEN bytes        -- see below
//    5. raw array data       K*4 little-endian doubles, row by row
//
//  The header dict is a Python-literal string, for a (K, 4) float64 array:
//
//      {'descr': '<f8', 'fortran_order': False, 'shape': (K, 4), }
//
//  with K substituted in. Two rules NumPy enforces on it:
//    - it ends with a newline '\n';
//    - it is padded with spaces (0x20) BEFORE that newline so that the total
//      header size  (6 + 2 + 2 + HEADER_LEN)  is a multiple of 64. HEADER_LEN
//      is the length of the padded dict string, newline included.
//
//  '<f8' means: little-endian ('<'), float ('f'), 8 bytes. On the lab
//  machines (x86-64, Apple Silicon) doubles are ALREADY stored little-endian,
//  so writing the raw bytes of each double gives you '<f8' for free. (On a
//  big-endian machine you would have to byte-swap -- note this in your
//  reflection.)
//
//  The body lives in npy_writer.cpp and is empty. Implement it.
// =============================================================

// Write `data` to `path` as a NumPy .npy v1.0 file: a 2-D (K, 4) float64
// C-order array, columns in kColumnNames order. Returns true on success,
// false if the file could not be opened. Open the stream in BINARY mode.
bool write_npy(const std::string& path, const DataSet& data);
