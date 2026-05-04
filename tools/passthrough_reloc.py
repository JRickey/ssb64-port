#!/usr/bin/env python3
"""
passthrough_reloc.py — produce a `.relo` resource by copying the bytes
verbatim from a Torch-extracted resource in BattleShip.o2r. Used for
relocData files the source-compile pipeline can't yet handle:

  - .spritelist-only files (no .c committed; need upstream's
    extractSpriteFile.py to generate the master)
  - .c-without-.reloc files (incomplete decomp scaffolding)
  - Bitfield-init typed structs (FTAttributes/WPAttributes/etc — IDO
    MSB-first packing differs from clang i686 LSB-first; needs PORT-
    guarded .c initializer rewrite first)
  - Symbol-resolution edge cases that still trip extract_inc_c.py

Passthrough output is byte-identical to what Torch would have emitted.
The runtime sees the same resource regardless of which archive
served it. Modders editing these files DON'T see source-edit changes —
the source-compile path is where edits flow through. To convert a
passthrough file to source-compiled, hook it through the normal
build_reloc_resource.py pipeline (which requires the file to have a
buildable .c + .reloc).

Usage:
    passthrough_reloc.py
        --battleship-o2r build/BattleShip.o2r
        --reloc-table    port/resource/RelocFileTable.cpp
        --file-id N
        --output         build/reloc_resources/<N>.relo
"""

from __future__ import annotations

import argparse
import re
import sys
import zipfile
from pathlib import Path


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--battleship-o2r", type=Path, required=True)
    ap.add_argument("--reloc-table", type=Path, required=True)
    ap.add_argument("--file-id", type=int, required=True)
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()

    table_re = re.compile(r'^\s*"([^"]+)",\s*/\*\s*(\d+)\s*\*/')
    table: dict[int, str] = {}
    for line in args.reloc_table.read_text().splitlines():
        m = table_re.match(line)
        if m:
            table[int(m.group(2))] = m.group(1)

    res_path = table.get(args.file_id)
    if res_path is None:
        print(f"error: file_id {args.file_id} not in {args.reloc_table}",
              file=sys.stderr)
        return 2

    zf = zipfile.ZipFile(args.battleship_o2r, "r")
    try:
        blob = zf.read(res_path)
    except KeyError:
        print(f"error: {res_path!r} not in {args.battleship_o2r}",
              file=sys.stderr)
        return 2

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(blob)
    print(f"passthrough {args.file_id} -> {args.output} ({len(blob)} bytes)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
