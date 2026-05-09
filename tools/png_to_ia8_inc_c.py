#!/usr/bin/env python3
"""
png_to_ia8_inc_c.py — convert RGBA PNGs to N64 IA8 raw bytes as a .tex.inc.c file.

IA8 is 8-bit-per-pixel: 4-bit intensity + 4-bit alpha. Suited for monochrome
sprites with anti-aliased edges (text, glyph atlases, B&W silhouettes).

Output format matches png_to_rgba16_inc_c.py's convention:
    0xHH, 0xHH, ...    (16 hex bytes per line)

Per-pixel encoding:
    pixel = (intensity_4bit << 4) | alpha_4bit
where intensity = (R + G + B) / 3 quantized to 4 bits, and alpha is the
alpha channel quantized to 4 bits.

Usage:
    png_to_ia8_inc_c.py <input.png> <output.tex.inc.c>
    png_to_ia8_inc_c.py --batch <input_dir> <output_dir>
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from PIL import Image


def png_to_ia8_bytes(png_path: Path) -> tuple[bytearray, int, int]:
    """Load an RGBA PNG and return (raw_ia8_bytes, width, height)."""
    img = Image.open(png_path).convert("RGBA")
    w, h = img.size
    px = img.load()
    raw = bytearray(w * h)

    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            i = (r + g + b) // 3
            i4 = (i >> 4) & 0xF
            a4 = (a >> 4) & 0xF
            raw[y * w + x] = (i4 << 4) | a4

    return raw, w, h


def write_inc_c(raw: bytes, output_path: Path, width: int, height: int) -> None:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(f"/* Auto-generated from PNG via png_to_ia8_inc_c.py — do not edit. */\n")
        f.write(f"/* Format: N64 IA8 ({width}x{height}, {len(raw)} bytes raw) */\n")
        for i in range(0, len(raw), 16):
            chunk = raw[i:i + 16]
            f.write("    " + ", ".join(f"0x{b:02X}" for b in chunk) + ",\n")


def convert_one(png_path: Path, out_path: Path) -> None:
    raw, w, h = png_to_ia8_bytes(png_path)
    write_inc_c(raw, out_path, w, h)
    print(f"  {png_path.name}  ({w}x{h}) -> {out_path.name}  [{len(raw)} bytes IA8]")


def main(argv: list[str]) -> int:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--batch", action="store_true", help="treat inputs as input_dir + output_dir")
    p.add_argument("inputs", nargs=2, metavar=("INPUT", "OUTPUT"))
    args = p.parse_args(argv)

    if args.batch:
        in_dir = Path(args.inputs[0])
        out_dir = Path(args.inputs[1])
        if not in_dir.is_dir():
            print(f"error: {in_dir} is not a directory", file=sys.stderr)
            return 1
        pngs = sorted(in_dir.glob("*.png"))
        if not pngs:
            print(f"error: no PNG files in {in_dir}", file=sys.stderr)
            return 1
        print(f"Converting {len(pngs)} PNGs from {in_dir} -> {out_dir}:")
        for png in pngs:
            out = out_dir / f"{png.stem}.tex.inc.c"
            convert_one(png, out)
        return 0
    else:
        png = Path(args.inputs[0])
        out = Path(args.inputs[1])
        if not png.is_file():
            print(f"error: {png} is not a file", file=sys.stderr)
            return 1
        convert_one(png, out)
        return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
