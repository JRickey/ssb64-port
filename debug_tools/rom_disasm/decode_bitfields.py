#!/usr/bin/env python3
"""Decode bitfield extraction patterns out of a disassembly listing.

Finds the idiomatic IDO sequence
    lw  $X, OFF($BASE)
    sll $Y, $X, K
    srl $Z, $Y, M     (or sra for signed)
and reports (offset, high_bit, low_bit, width, signed, dest_store_if_any).

Feed it the text output of disasm.py.
"""
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

RE_INSTR = re.compile(
    r"^([0-9a-f]{8}): ([0-9a-f]{8})\s+(\w+)\s+(.*?)\s*$"
)


def parse_operand(text: str) -> list[str]:
    return [t.strip() for t in text.split(",")]


def decode(path: Path, base_reg: str) -> None:
    lines = [l.rstrip() for l in path.read_text().splitlines() if RE_INSTR.match(l)]

    instrs = []
    for l in lines:
        m = RE_INSTR.match(l)
        assert m
        vram = int(m.group(1), 16)
        op = m.group(3).lower()
        ops = parse_operand(m.group(4))
        instrs.append((vram, op, ops))

    base_re = re.compile(r"^(-?0x[0-9A-Fa-f]+|\d+)\((\$\w+)\)$")

    findings = []
    n = len(instrs)
    for i, (vram, op, ops) in enumerate(instrs):
        if op != "lw" or len(ops) != 2:
            continue
        m = base_re.match(ops[1])
        if not m or m.group(2) != base_reg:
            continue
        off = int(m.group(1), 16) if m.group(1).lower().startswith(("0x", "-0x")) else int(m.group(1))
        if off < 0:
            off &= 0xFFFF
        dest_reg = ops[0]

        sll_k = None
        after_reg = dest_reg
        shr_k = None
        shr_signed = False
        store_dest = None
        for j in range(i + 1, min(i + 16, n)):
            vj, opj, oj = instrs[j]
            if opj == "sll" and len(oj) == 3 and oj[1] == after_reg:
                try:
                    sll_k = int(oj[2].replace("0x", ""), 16) if "0x" in oj[2] else int(oj[2])
                except ValueError:
                    continue
                after_reg = oj[0]
            elif opj in ("srl", "sra") and len(oj) == 3 and oj[1] == after_reg:
                try:
                    shr_k = int(oj[2].replace("0x", ""), 16) if "0x" in oj[2] else int(oj[2])
                except ValueError:
                    continue
                shr_signed = opj == "sra"
                break
            elif opj == "srl" and len(oj) == 3 and sll_k is None:
                # srl without a preceding sll -> pure right shift (width = 32 - shr_k)
                try:
                    shr_k = int(oj[2].replace("0x", ""), 16) if "0x" in oj[2] else int(oj[2])
                except ValueError:
                    continue
                if oj[1] == dest_reg:
                    break
                else:
                    shr_k = None
                    break
            else:
                # stop if the dest register is clobbered or used elsewhere first
                if opj in ("sw", "sh", "sb") and oj[0] == after_reg:
                    # stored directly as u32 -> full-word field
                    store_dest = oj[1]
                    break
                continue
        # look for a store in the next ~6 instructions that uses the shifted result
        if shr_k is not None:
            for j in range(i + 1, min(i + 20, n)):
                vj, opj, oj = instrs[j]
                if opj in ("sw", "sh", "sb", "swc1", "sc", "mtc1") and len(oj) >= 2 and oj[0] == after_reg:
                    store_dest = oj[1]
                    break

        if sll_k is not None and shr_k is not None:
            # bits [31-K : 31-K-W+1] where W = 32 - shr_k
            width = 32 - shr_k
            high = 31 - sll_k
            low = high - width + 1
            findings.append((vram, off, high, low, width, shr_signed, store_dest))
        elif sll_k is None and shr_k is None:
            # No shift sequence -> full 32-bit value. Probably pointer load.
            findings.append((vram, off, 31, 0, 32, False, store_dest))

    by_off: dict[int, list] = {}
    for vr, off, hi, lo, w, signed, dest in findings:
        by_off.setdefault(off, []).append((vr, hi, lo, w, signed, dest))

    print(f"# Bitfield extractions from {base_reg}:")
    for off in sorted(by_off):
        entries = by_off[off]
        print(f"\n  offset 0x{off:02X}:")
        for vr, hi, lo, w, signed, dest in entries:
            sign_char = "s" if signed else "u"
            if w == 32:
                print(f"    {vr:08x}  [31:0]       ({sign_char}32)   -> {dest}")
            else:
                print(f"    {vr:08x}  [{hi:2}:{lo:2}]  w={w:2} ({sign_char})      -> {dest}")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("disasm_file")
    ap.add_argument("--base", default="$v1")
    args = ap.parse_args()
    decode(Path(args.disasm_file), args.base)


if __name__ == "__main__":
    main()
