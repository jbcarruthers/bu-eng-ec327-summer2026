# Copyright 2026 J. Carruthers <jbc@bu.edu>
# SPDX-License-Identifier: MIT
# Purpose: tiny x86-flavored ISA simulator — the clean on-ramp to real
#          assembly before students meet g++ -S output.
# Topic: layered-machine
# Category: tool
# Lecture: L8,L9
#
# A deliberately small teaching machine. Intel-style syntax (dst, src order;
# no %/$ sigils), real x86-64 register names (rax, rbx, ... rsp), a flat
# byte-addressed memory with an 8-byte word, two flags (ZF, SF), and ~15
# instructions. Enough to run arithmetic, branches, loops, and recursive
# calls (push/pop/call/ret) so the call stack can be *watched* growing.
#
# This is the canonical, testable engine. The lecture-8 marimo notebook
# (notes/topic4_machine/lecture_08_machine_sim.py) embeds a copy of this
# same code so the published WebAssembly build stays self-contained; keep
# the two in sync. Run `python -u simplesim.py` to execute the built-in
# self-tests.
#
# Honest simplifications (state these to students):
#   - one word = 8 bytes, every value is a signed integer, no overflow model
#   - flags are ZF (zero) and SF (sign) only; jl/jg assume no overflow
#   - the program counter indexes instructions, not bytes; call/ret push and
#     pop a "return address" that is really the next instruction's index
#   - no heap / malloc here on purpose — the heap lives in the GDB half of
#     L9, on a real binary. This machine owns the *stack* story.

from __future__ import annotations

from dataclasses import dataclass, field

WORD = 8  # bytes per word; addresses move in multiples of WORD
GP_REGS = ["rax", "rbx", "rcx", "rdx"]  # general purpose
ALL_REGS = GP_REGS + ["rsp"]  # rsp = stack pointer
STACK_TOP = 0x200  # rsp starts here and grows *down*

# Instructions that take (dst, src); dst may be a register or [mem].
_BINOPS = {"mov", "add", "sub", "imul", "cmp"}
# Instructions that take a single label operand.
_JUMPS = {"jmp", "je", "jne", "jl", "jg", "jle", "jge"}
# Instructions that take a single register/value operand.
_UNARY = {"inc", "dec", "neg", "push", "pop", "call"}
# Instructions that take no operand.
_NULLARY = {"ret", "hlt", "nop"}


class AsmError(Exception):
    """Raised on a malformed program or an illegal operation at run time."""


@dataclass
class Instr:
    op: str
    args: tuple
    line_no: int  # 1-based source line, for error messages and highlighting
    text: str  # original source text of the instruction


@dataclass
class Machine:
    """A single run of the toy machine. Build with `Machine.load(source)`."""

    program: list = field(default_factory=list)  # list[Instr]
    labels: dict = field(default_factory=dict)  # name -> instruction index
    regs: dict = field(default_factory=dict)  # reg name -> int
    mem: dict = field(default_factory=dict)  # byte address -> int
    pc: int = 0
    zf: bool = False
    sf: bool = False
    halted: bool = False
    steps: int = 0

    # ---- construction -----------------------------------------------------

    @classmethod
    def load(cls, source: str) -> "Machine":
        program, labels = _parse(source)
        m = cls(program=program, labels=labels)
        m.regs = {r: 0 for r in ALL_REGS}
        m.regs["rsp"] = STACK_TOP
        return m

    # ---- operand evaluation ----------------------------------------------

    def _read(self, tok: str) -> int:
        """Resolve a source operand to an integer value."""
        if tok in ALL_REGS:
            return self.regs[tok]
        if tok.startswith("["):
            return self.mem.get(self._addr(tok), 0)
        return _imm(tok)

    def _write(self, tok: str, value: int) -> None:
        """Store `value` into a destination operand (register or memory)."""
        if tok in ALL_REGS:
            self.regs[tok] = value
        elif tok.startswith("["):
            self.mem[self._addr(tok)] = value
        else:
            raise AsmError(f"cannot write to '{tok}' (not a register or [mem])")

    def _addr(self, tok: str) -> int:
        """Resolve a `[reg]`, `[reg+disp]`, `[reg-disp]`, or `[imm]` operand
        to a concrete byte address."""
        inner = tok[1:-1].strip()
        for sign, op in (("+", 1), ("-", -1)):
            if sign in inner:
                base, disp = inner.split(sign, 1)
                base, disp = base.strip(), disp.strip()
                base_val = self.regs[base] if base in ALL_REGS else _imm(base)
                return base_val + op * _imm(disp)
        if inner in ALL_REGS:
            return self.regs[inner]
        return _imm(inner)

    # ---- flags ------------------------------------------------------------

    def _set_flags(self, result: int) -> None:
        self.zf = result == 0
        self.sf = result < 0

    # ---- one step ---------------------------------------------------------

    def step(self) -> None:
        """Execute the instruction at `pc`. Advances `pc`, or follows a jump,
        or halts. Safe to call on a halted machine (no-op)."""
        if self.halted:
            return
        if not (0 <= self.pc < len(self.program)):
            self.halted = True
            return

        ins = self.program[self.pc]
        op, args = ins.op, ins.args
        next_pc = self.pc + 1  # default: fall through

        if op == "mov":
            self._write(args[0], self._read(args[1]))
        elif op == "add":
            r = self._read(args[0]) + self._read(args[1])
            self._write(args[0], r)
            self._set_flags(r)
        elif op == "sub":
            r = self._read(args[0]) - self._read(args[1])
            self._write(args[0], r)
            self._set_flags(r)
        elif op == "imul":
            r = self._read(args[0]) * self._read(args[1])
            self._write(args[0], r)
            self._set_flags(r)
        elif op == "inc":
            r = self._read(args[0]) + 1
            self._write(args[0], r)
            self._set_flags(r)
        elif op == "dec":
            r = self._read(args[0]) - 1
            self._write(args[0], r)
            self._set_flags(r)
        elif op == "neg":
            r = -self._read(args[0])
            self._write(args[0], r)
            self._set_flags(r)
        elif op == "cmp":
            self._set_flags(self._read(args[0]) - self._read(args[1]))
        elif op in _JUMPS:
            if self._jump_taken(op):
                next_pc = self._target(args[0])
        elif op == "push":
            self.regs["rsp"] -= WORD
            self.mem[self.regs["rsp"]] = self._read(args[0])
        elif op == "pop":
            self._write(args[0], self.mem.get(self.regs["rsp"], 0))
            self.regs["rsp"] += WORD
        elif op == "call":
            self.regs["rsp"] -= WORD
            self.mem[self.regs["rsp"]] = self.pc + 1  # return address
            next_pc = self._target(args[0])
        elif op == "ret":
            next_pc = self.mem.get(self.regs["rsp"], len(self.program))
            self.regs["rsp"] += WORD
        elif op == "hlt":
            self.halted = True
            return
        elif op == "nop":
            pass
        else:  # pragma: no cover - parser already rejects unknowns
            raise AsmError(f"unknown instruction '{op}'")

        self.pc = next_pc
        self.steps += 1
        if not (0 <= self.pc < len(self.program)):
            self.halted = True

    def _jump_taken(self, op: str) -> bool:
        if op == "jmp":
            return True
        if op == "je":
            return self.zf
        if op == "jne":
            return not self.zf
        if op == "jl":
            return self.sf and not self.zf
        if op == "jge":
            return not self.sf or self.zf
        if op == "jg":
            return not self.sf and not self.zf
        if op == "jle":
            return self.sf or self.zf
        return False  # pragma: no cover

    def _target(self, label: str) -> int:
        if label not in self.labels:
            raise AsmError(f"jump/call to undefined label '{label}'")
        return self.labels[label]

    # ---- whole-program run ------------------------------------------------

    def run(self, max_steps: int = 10_000) -> "Machine":
        """Step until halt or the step budget is exhausted (guards against
        runaway loops in the UI)."""
        while not self.halted and self.steps < max_steps:
            self.step()
        return self

    def stack_view(self):
        """Return [(address, value), ...] for the live stack region,
        top-of-stack first. Handy for the notebook's stack panel."""
        rsp = self.regs["rsp"]
        return [(a, self.mem[a]) for a in sorted(self.mem) if rsp <= a < STACK_TOP]


# --------------------------------------------------------------------------
# parsing
# --------------------------------------------------------------------------


def _imm(tok: str) -> int:
    """Parse an immediate: decimal or 0x-hex. Raises AsmError on garbage."""
    try:
        return int(tok, 0)
    except ValueError as exc:
        raise AsmError(f"expected a number, register, or [mem], got '{tok}'") from exc


def _split_operands(rest: str) -> tuple:
    """Split an operand string on commas, respecting nothing fancy — our
    operands never contain commas. Empty string -> ()."""
    rest = rest.strip()
    if not rest:
        return ()
    return tuple(part.strip() for part in rest.split(","))


def _parse(source: str):
    """Two-pass assembler: collect labels, then build the instruction list.
    Returns (program, labels)."""
    program = []
    labels = {}

    for line_no, raw in enumerate(source.splitlines(), start=1):
        # strip comments: everything after ';' or '#'
        line = raw.split(";", 1)[0].split("#", 1)[0].strip()
        if not line:
            continue

        # one or more leading "label:" tokens, then maybe an instruction
        while ":" in line:
            label, _, line = line.partition(":")
            label = label.strip()
            if not label or " " in label:
                raise AsmError(f"line {line_no}: bad label '{label}'")
            if label in labels:
                raise AsmError(f"line {line_no}: duplicate label '{label}'")
            labels[label] = len(program)
            line = line.strip()

        if not line:
            continue

        parts = line.split(None, 1)
        op = parts[0].lower()
        args = _split_operands(parts[1] if len(parts) > 1 else "")

        _check_arity(op, args, line_no)
        program.append(Instr(op=op, args=args, line_no=line_no, text=line))

    # validate jump/call targets up front for friendlier errors
    for ins in program:
        if ins.op in _JUMPS or ins.op == "call":
            if ins.args[0] not in labels:
                raise AsmError(
                    f"line {ins.line_no}: {ins.op} to undefined label "
                    f"'{ins.args[0]}'"
                )
    return program, labels


def _check_arity(op: str, args: tuple, line_no: int) -> None:
    if op in _BINOPS:
        want = 2
    elif op in _JUMPS or op in _UNARY:
        want = 1
    elif op in _NULLARY:
        want = 0
    else:
        raise AsmError(f"line {line_no}: unknown instruction '{op}'")
    if len(args) != want:
        raise AsmError(
            f"line {line_no}: '{op}' takes {want} operand(s), got {len(args)}"
        )


# --------------------------------------------------------------------------
# canonical example programs (shared by the notebook and the self-tests)
# --------------------------------------------------------------------------

EXAMPLES = {
    "sum": """\
; int sum(int a, int b) { return a + b; }
        mov rax, 5
        mov rbx, 7
        add rax, rbx      ; rax = a + b  -> result in rax
        hlt
""",
    "max": """\
; int max(int a, int b) { return a > b ? a : b; }
        mov rax, 3        ; a
        mov rbx, 9        ; b
        cmp rax, rbx      ; sets ZF/SF from a - b
        jge done          ; a >= b ?  keep a
        mov rax, rbx      ; else result = b
done:
        hlt               ; result in rax
""",
    "factorial_loop": """\
; iterative factorial of n
        mov rcx, 5        ; n
        mov rax, 1        ; acc = 1
loop:
        cmp rcx, 1
        jle done          ; while (n > 1)
        imul rax, rcx     ;   acc *= n
        dec rcx           ;   n--
        jmp loop
done:
        hlt               ; n! in rax
""",
    "factorial_rec": """\
; recursive factorial — watch the stack grow.
; convention: argument in rcx, result in rax.
        mov rcx, 4
        call fact
        hlt
fact:                     ; rcx = n
        cmp rcx, 1
        jg recurse
        mov rax, 1        ; base case: 0! = 1! = 1
        ret
recurse:
        push rcx          ; save n across the recursive call
        dec rcx           ; n - 1
        call fact         ; rax = (n-1)!
        pop rcx           ; restore n
        imul rax, rcx     ; rax = n * (n-1)!
        ret
""",
    "memory": """\
; pointers, previewed: a register can hold an *address*.
; 0x300 sits in data memory (above the stack, which grows down from 0x200).
        mov rbx, 0x300    ; rbx is a base address (a pointer)
        mov rax, 10
        mov [rbx], rax    ; store 10 at mem[0x300]  (*p = 10)
        mov rcx, [rbx]    ; load it back            (x = *p)
        add rcx, 5
        mov [rbx+8], rcx  ; store 15 at mem[0x308]  (p[1] = 15)
        hlt
""",
}


# --------------------------------------------------------------------------
# self-tests
# --------------------------------------------------------------------------


def _selftest() -> None:
    checks = []

    m = Machine.load(EXAMPLES["sum"]).run()
    checks.append(("sum -> rax == 12", m.regs["rax"] == 12))

    m = Machine.load(EXAMPLES["max"]).run()
    checks.append(("max(3,9) -> rax == 9", m.regs["rax"] == 9))

    m = Machine.load(EXAMPLES["factorial_loop"]).run()
    checks.append(("5! loop -> rax == 120", m.regs["rax"] == 120))

    m = Machine.load(EXAMPLES["factorial_rec"]).run()
    checks.append(("4! recursive -> rax == 24", m.regs["rax"] == 24))
    checks.append(("recursion restores rsp", m.regs["rsp"] == STACK_TOP))

    m = Machine.load(EXAMPLES["memory"]).run()
    checks.append(("store/load -> mem[0x300] == 10", m.mem.get(0x300) == 10))
    checks.append(("displacement -> mem[0x308] == 15", m.mem.get(0x308) == 15))

    # a hand-written program exercising every jump
    branch = """\
        mov rax, 0
        cmp rax, 0
        jne bad
        je  ok
bad:
        mov rax, 999
ok:
        hlt
"""
    m = Machine.load(branch).run()
    checks.append(("je/jne -> rax == 0", m.regs["rax"] == 0))

    # parser error surfaces cleanly
    bad_label = False
    try:
        Machine.load("        jmp nowhere\n        hlt\n")
    except AsmError:
        bad_label = True
    checks.append(("undefined label rejected", bad_label))

    width = max(len(name) for name, _ in checks)
    all_ok = True
    for name, ok in checks:
        all_ok = all_ok and ok
        print(f"  [{'PASS' if ok else 'FAIL'}] {name:<{width}}")
    print(f"\n{'ALL PASS' if all_ok else 'FAILURES PRESENT'} "
          f"({sum(ok for _, ok in checks)}/{len(checks)})")
    if not all_ok:
        raise SystemExit(1)


if __name__ == "__main__":
    _selftest()
