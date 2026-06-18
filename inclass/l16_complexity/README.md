# L16 — Complexity & Optimization: in-class demos + activity

Material for Lecture 16 (Wed 2026-06-17). Build everything at **`-O2`** —
timing a `-O0` build measures the wrong program.

## 1. Live demo — "measure, don't guess": the timing race

```sh
g++ -std=c++20 -O2 -Wall -Wextra race.cpp -o race && ./race
```

Two functions answer the **same** question ("does this vector have a
duplicate?") — a nested-loop O(n²) and a sort-based O(n log n). Times both on
the same data at growing `n` and prints a table. Watch the O(n²) column blow up
(~4× per doubling of `n`) while O(n log n) barely moves (~2×). **Big-O, on the
clock.**

Companion footgun demo — the quadratic string build from the opener:

```sh
g++ -std=c++20 -O2 -Wall -Wextra string_build.cpp -o sb && ./sb
```

`out = out + p` (O(n²)) vs `out += p` (O(n)). One character of difference,
a cliff in the timing. "Schlemiel the Painter" made concrete.

## 2. Activity — guess the big-O

```sh
g++ -std=c++20 -Wall -Wextra bigo_triage.cpp -o bigo && ./bigo
```

Five functions. In pairs, **write the big-O of each before looking at the
answer**, reasoning from the code shape (one pass → O(n); loop-in-loop → O(n²);
halving → O(log n); fixed steps → O(1)). The program runs and prints each
result so you can call them; the costs are in `SOLUTIONS.md` (instructor-only —
not published).

Then close the loop: pick the one O(n²) function in the set and name the
**sort-or-hash** move that would drop it to O(n log n) or O(n).

## Build all

```sh
for f in race string_build bigo_triage; do
  g++ -std=c++20 -O2 -Wall -Wextra "$f.cpp" -o "${f%%_*}" || echo "FAILED: $f"
done
```
