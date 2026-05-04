#!/usr/bin/env python3
"""
pack_reloc_archive.py — pack per-file `.relo` resources produced by
build_reloc_resource.py into a single .o2r archive (zip) at the resource
paths the runtime expects.

Usage:
    pack_reloc_archive.py
        --manifest <build>/reloc_fromsource_manifest.txt
        --reloc-dir <build>/reloc_resources
        --output    <build>/BattleShip.fromsource.o2r

Manifest format: one line per file, `<file_id>|<resource_path>`.
"""

from __future__ import annotations

import argparse
import sys
import zipfile
from pathlib import Path


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--manifest", type=Path, required=True)
    ap.add_argument("--reloc-dir", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()

    args.output.parent.mkdir(parents=True, exist_ok=True)

    n = 0
    with zipfile.ZipFile(args.output, "w", zipfile.ZIP_DEFLATED) as zf:
        for line in args.manifest.read_text().splitlines():
            line = line.strip()
            if not line:
                continue
            try:
                fid_s, path = line.split("|", 1)
                fid = int(fid_s.strip())
                path = path.strip()
            except ValueError:
                print(f"warning: bad manifest line: {line!r}", file=sys.stderr)
                continue
            relo = args.reloc_dir / f"{fid}.relo"
            if not relo.is_file():
                print(f"error: missing {relo}", file=sys.stderr)
                return 2
            zf.writestr(path, relo.read_bytes())
            n += 1

    print(f"packed {n} entries into {args.output}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
