#!/usr/bin/env python3
"""
gen_reloc_cmake.py — walk decomp/src/relocData/ and emit a CMake include
file with one add_reloc_resource() call per file we can build from source.

Output goes to <build>/reloc_data_targets.cmake. The top-level CMakeLists
includes() it after RelocData.cmake. Re-running the script is idempotent
and side-effect-free (the output is regenerated on each cmake configure).

Eligibility filter (M3 best-effort):
  - .c file exists (skip JP-only files where only `<id>_<name>.jp.c` is present)
  - .reloc file exists (skip files with no relocation metadata)
  - .c does not #include any *.inc.c file (those need upstream's
    `make extract` step which the port doesn't run)
  - .c does not initialize any *Attributes struct or *GroundData struct
    (those use IDO-style bitfield initializers that clang i686 packs to
    different total sizes — needs per-field PORT-guarded rewrite of the
    initializer first; tracked as a follow-up)

Externs are NOT a blocker: tools/annotate_externs_from_torch.py has
populated `# -> file N (Name)` on every US extern line, and
tools/build_reloc_resource.py reads those annotations to populate
extern_file_ids[].

Usage:
    gen_reloc_cmake.py
        --reloc-dir   <decomp/src/relocData>
        --reloc-table <port/resource/RelocFileTable.cpp>
        --output      <build>/reloc_data_targets.cmake
        [--report     <build>/reloc_data_eligibility.json]
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

# Matches a line `	"reloc_<cat>/<Name>", /* N */`
TABLE_RE = re.compile(r'^\s*"([^"]+)",\s*/\*\s*(\d+)\s*\*/')

# Matches `<id>_<Name>.c` filename
RELOCDATA_FILE_RE = re.compile(r"^(\d+)_([A-Za-z0-9_]+)\.c$")


def parse_reloc_table(path: Path) -> dict[int, str]:
    out: dict[int, str] = {}
    for line in path.read_text().splitlines():
        m = TABLE_RE.match(line)
        if m:
            out[int(m.group(2))] = m.group(1)
    return out


def has_inc_c_include(c_path: Path) -> bool:
    """True if the .c file #includes any *.inc.c — those require upstream's
    make-extract step which the port doesn't run."""
    try:
        text = c_path.read_text()
    except Exception:
        return True  # be conservative; treat as ineligible
    return bool(re.search(r'#\s*include\s+[<"][^>"]*\.inc\.c[>"]', text))


_BITFIELD_TYPE_RE = re.compile(
    r"\b(FTAttributes|WPAttributes|ITAttributes|MPGroundData)\s+\w+\s*=", re.M)

# Macros only defined in headers upstream's `make extract` generates
# (e.g. build/<v>/src/relocData/motiondesc_offsets.h). MainMotion files
# use them as initializers; until we extract / vendor them, these files
# can't compile.
_GENERATED_MACRO_RE = re.compile(
    r"\b(ftMotionCommand\w+|aobjEvent32End|aobjEvent16End)\s*\(", re.M)


def uses_bitfield_initializer(c_path: Path) -> bool:
    """True if the file initializes one of the bitfield-heavy struct types
    whose PORT-guarded layout differs from upstream's IDO layout. clang
    i686 packs LSB-first; IDO packs MSB-first into pad gaps. Until the
    .c initializers themselves are PORT-guarded, source-compile of these
    files produces a different data_size than Torch's ROM-extracted bytes.
    """
    try:
        return bool(_BITFIELD_TYPE_RE.search(c_path.read_text()))
    except Exception:
        return True  # be conservative


def uses_generated_macros(c_path: Path) -> bool:
    """True if the file uses macros only defined in upstream-generated
    headers (motion command DSL, AObjEvent32 helpers). The port doesn't
    run upstream's `make extract`, so these macros are undefined → clang
    treats them as implicit function declarations → compile fails.
    """
    try:
        return bool(_GENERATED_MACRO_RE.search(c_path.read_text()))
    except Exception:
        return True


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--reloc-dir", type=Path, required=True)
    ap.add_argument("--reloc-table", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    ap.add_argument("--report", type=Path, default=None)
    args = ap.parse_args()

    table = parse_reloc_table(args.reloc_table)
    if not table:
        print(f"error: no entries parsed from {args.reloc_table}", file=sys.stderr)
        return 2

    eligible: list[tuple[int, str, str]] = []  # (file_id, c_path_rel, resource_path)
    skipped: dict[str, list[int]] = {
        "no_c_file": [],
        "no_reloc_file": [],
        "no_table_entry": [],
        "needs_inc_c": [],
        "uses_bitfield_init": [],
        "uses_generated_macros": [],
    }

    for c_path in sorted(args.reloc_dir.glob("*.c")):
        m = RELOCDATA_FILE_RE.match(c_path.name)
        if not m:
            continue
        fid = int(m.group(1))

        reloc_path = c_path.with_suffix(".reloc")
        if not reloc_path.exists():
            skipped["no_reloc_file"].append(fid)
            continue

        if fid not in table:
            skipped["no_table_entry"].append(fid)
            continue

        if has_inc_c_include(c_path):
            skipped["needs_inc_c"].append(fid)
            continue

        if uses_bitfield_initializer(c_path):
            skipped["uses_bitfield_init"].append(fid)
            continue

        if uses_generated_macros(c_path):
            skipped["uses_generated_macros"].append(fid)
            continue

        eligible.append((fid, c_path.name, table[fid]))

    # Emit CMake include
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w") as f:
        f.write("# Auto-generated by tools/gen_reloc_cmake.py — do not edit.\n")
        f.write(f"# {len(eligible)} eligible relocData files of {len(eligible) + sum(len(v) for v in skipped.values())} total.\n\n")
        for fid, basename, res_path in eligible:
            f.write(
                f'add_reloc_resource({fid} '
                f'${{CMAKE_SOURCE_DIR}}/decomp/src/relocData/{basename} '
                f'"{res_path}")\n'
            )

    print(f"emitted {args.output} ({len(eligible)} eligible)")
    print(f"  skipped — no .c (JP-only files):    {len(skipped['no_c_file'])}")
    print(f"  skipped — no .reloc:                {len(skipped['no_reloc_file'])}")
    print(f"  skipped — no table entry:           {len(skipped['no_table_entry'])}")
    print(f"  skipped — needs upstream .inc.c:    {len(skipped['needs_inc_c'])}")
    print(f"  skipped — bitfield-init struct:     {len(skipped['uses_bitfield_init'])}")
    print(f"  skipped — uses generated macros:    {len(skipped['uses_generated_macros'])}")

    if args.report is not None:
        args.report.write_text(json.dumps(
            {"eligible_count": len(eligible),
             "eligible": [fid for fid, _, _ in eligible],
             **skipped},
            indent=2))
        print(f"wrote eligibility report {args.report}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
