#!/usr/bin/env python3
"""Disassemble a function out of baserom.us.z64 and surface struct-pointer loads.

Usage:
    disasm.py <vram-hex> [--rom PATH] [--count N] [--base REG]

Resolves VRAM -> ROM via a hard-coded segment table (ovl2, ovl3, main), reads N
instructions, prints their disassembly, and separately summarizes every load
whose base register matches --base (default $a1). The summary is the ground
truth for "which struct offsets does the original game actually read?" — which
is what the ITAttributes handoff needs.
"""
from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

import rabbitizer

# ROM start, VRAM start. Covers every code segment needed for this audit.
# Extracted from references/ssb-decomp-re/smashbrothers.us.yaml.
SEGMENTS = [
    ("main",     0x001060, 0x80000460, 0x040000 - 0x001060),
    ("ovl0",     0x043B20, 0x8008B880, 0x051C90 - 0x043B20),
    ("ovl2",     0x051C90, 0x800D6490, 0x0AC540 - 0x051C90),
    ("ovl3",     0x0AC540, 0x80131B00, 0x107620 - 0x0AC540),
]

LOAD_INSTRS = {"lw", "lh", "lhu", "lb", "lbu", "ld", "lwc1", "ldc1"}


def vram_to_rom(vram: int) -> int:
    for name, rom, vstart, size in SEGMENTS:
        if vstart <= vram < vstart + size:
            return rom + (vram - vstart)
    raise SystemExit(f"VRAM {vram:#010x} not in any known segment")


def disassemble(rom_path: Path, vram: int, count: int, base_reg: str) -> None:
    data = rom_path.read_bytes()
    rom = vram_to_rom(vram)
    print(f"# VRAM {vram:#010x} -> ROM {rom:#x}")
    print(f"# {count} instrs, base register filter: {base_reg}\n")

    loads: list[tuple[int, str, int]] = []  # (vram, mnemonic, offset)
    for i in range(count):
        word = struct.unpack(">I", data[rom + i * 4 : rom + i * 4 + 4])[0]
        instr_vram = vram + i * 4
        instr = rabbitizer.Instruction(word, vram=instr_vram)
        disasm = instr.disassemble(extraLJust=20)
        print(f"{instr_vram:08x}: {word:08x}  {disasm}")
        m = instr.getOpcodeName().lower()
        if m in LOAD_INSTRS:
            rs_name = "$" + instr.rs.name
            if rs_name == base_reg:
                off = instr.getProcessedImmediate()
                loads.append((instr_vram, m, off))

    if loads:
        print(f"\n# Loads from {base_reg} (candidate struct-field reads):")
        by_off: dict[int, list[tuple[int, str]]] = {}
        for vr, mn, off in loads:
            by_off.setdefault(off, []).append((vr, mn))
        for off in sorted(by_off):
            entries = by_off[off]
            mnems = ", ".join(sorted({mn for _, mn in entries}))
            vrams = ", ".join(f"{vr:08x}" for vr, _ in entries)
            print(f"  +0x{off & 0xFFFF:04x}  ({mnems:10}) @ {vrams}")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("vram", help="VRAM hex address of function start (e.g. 0x8016e174)")
    ap.add_argument("--rom", default="baserom.us.z64", help="ROM path")
    ap.add_argument("--count", type=int, default=120, help="instructions to disassemble")
    ap.add_argument("--base", default="$a1", help="base register to filter loads on")
    args = ap.parse_args()

    vram = int(args.vram, 16)
    disassemble(Path(args.rom), vram, args.count, args.base)


if __name__ == "__main__":
    main()
