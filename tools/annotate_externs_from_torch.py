#!/usr/bin/env python3
"""
annotate_externs_from_torch.py — port-side equivalent of upstream's
decomp/tools/annotateExternRelocFids.py, using Torch's already-extracted
BattleShip.o2r as the source of truth instead of upstream's
vpk0_excess_bytes.txt + IDO-compiled .o files.

Why this exists
---------------
~43% of `extern` lines in decomp/src/relocData/*.reloc lack the
`# -> file N (Name)` annotation that maps each chain slot to its target
file_id. Without those, the compile-from-source pipeline can't populate
`extern_file_ids[]` for many files. Upstream's annotator solves the
problem but requires the IDO/MIPS toolchain, which is not part of the
port's build dependency surface (clang/MSVC/Python/CMake only).

Algorithm
---------
Empirical observation across 130 fully-annotated .reloc files: the
**source order of extern entries matches Torch's chain-walk order**
(99.2% match rate — 130/131 files; 54/65 of partially-annotated files
also match). So pairing the i-th source-order extern with
extern_file_ids[i] from Torch is correct except for a handful of
edge cases (duplicate ptr/target pairs, stale upstream annotations).

For each .reloc file with externs:
  1. Look up the corresponding resource in BattleShip.o2r via the port's
     RelocFileTable.cpp.
  2. Parse the resource → extern_file_ids[] (chain order from Torch).
  3. For each source-order extern entry i:
     - If the count of source entries == count of Torch entries, pair
       source[i] with extern_file_ids[i].
     - If existing annotation matches: pass-through.
     - If existing annotation conflicts: flag for review (don't overwrite).
     - If no annotation: write `# -> file N (Name)` with name from
       relocFileDescriptions.us.txt.
  4. If counts differ, flag the whole file for manual review.

Modder workflow
---------------
A modder editing .c files in decomp/src/relocData/ adds new extern
entries to .reloc. They re-run this tool — annotations get populated
from Torch. For NEW symbols / NEW files the modder is creating that
don't exist in BattleShip.o2r, the modder annotates manually with
`# -> file N` first. The tool's --check mode verifies alignment without
writing, useful for CI.
"""

from __future__ import annotations

import argparse
import re
import struct
import sys
import zipfile
from dataclasses import dataclass
from pathlib import Path

HEADER_SIZE = 0x40

TABLE_RE = re.compile(r'^\s*"([^"]+)",\s*/\*\s*(\d+)\s*\*/')
RELOC_FILE_RE = re.compile(r"^(\d+)_([A-Za-z0-9_]+)\.reloc$")
EXTERN_LINE_RE = re.compile(r"^extern\s+(\S+)\s+(\S+)(.*)$")


@dataclass
class TorchExterns:
    file_id: int
    extern_file_ids: list[int]


def parse_reloc_table(path: Path) -> dict[int, str]:
    out: dict[int, str] = {}
    for line in path.read_text().splitlines():
        m = TABLE_RE.match(line)
        if m:
            out[int(m.group(2))] = m.group(1)
    return out


def parse_file_names(descriptions_path: Path) -> dict[int, str]:
    """Extract file_id -> short name from the FILE NAMES section
    (`-NNN: Name` lines) of decomp/tools/relocFileDescriptions.us.txt."""
    out: dict[int, str] = {}
    line_re = re.compile(r"^-(\d+):\s*(\S+)\s*$")
    in_names = False
    for line in descriptions_path.read_text().splitlines():
        if "FILE NAMES" in line:
            in_names = True
            continue
        if in_names and line.startswith("# FILE CONTENTS"):
            break
        m = line_re.match(line)
        if m:
            out[int(m.group(1))] = m.group(2)
    return out


def torch_externs(zf: zipfile.ZipFile, table: dict[int, str], file_id: int
                  ) -> TorchExterns | None:
    path = table.get(file_id)
    if path is None:
        return None
    try:
        blob = zf.read(path)
    except KeyError:
        return None
    off = HEADER_SIZE
    if len(blob) < off + 12:
        return None
    fid, = struct.unpack_from("<I", blob, off); off += 4
    _, = struct.unpack_from("<H", blob, off); off += 2
    _, = struct.unpack_from("<H", blob, off); off += 2
    n_ext, = struct.unpack_from("<I", blob, off); off += 4
    if len(blob) < off + 2 * n_ext:
        return None
    ext_ids = list(struct.unpack_from(f"<{n_ext}H", blob, off))
    return TorchExterns(file_id=fid, extern_file_ids=ext_ids)


@dataclass
class AnnotationOutcome:
    file_id: int
    fname: str
    written: int = 0       # newly-annotated lines
    matched: int = 0       # existing annotations confirmed correct
    conflict: int = 0      # existing annotations differ from Torch
    skipped_reason: str | None = None   # whole-file skip


def annotate_one(reloc_path: Path,
                 torch: TorchExterns,
                 names: dict[int, str],
                 *,
                 check_only: bool,
                 ) -> AnnotationOutcome:
    out = AnnotationOutcome(file_id=torch.file_id, fname=reloc_path.name)
    lines = reloc_path.read_text().splitlines()

    # Find every extern line and parse it.
    extern_indices: list[int] = []
    extern_meta: list[tuple[str, str, str | None]] = []
    # (ptr_label, target_str, existing_annotation_or_None)
    for i, line in enumerate(lines):
        m = EXTERN_LINE_RE.match(line.strip())
        if m is None:
            continue
        ptr_label, target_str, trailing = m.group(1), m.group(2), m.group(3)
        annot_match = re.search(r"#\s*->\s*file\s*(\d+)", trailing)
        existing = annot_match.group(1) if annot_match else None
        extern_indices.append(i)
        extern_meta.append((ptr_label, target_str, existing))

    if not extern_meta:
        return out  # nothing to do

    n_src = len(extern_meta)
    n_torch = len(torch.extern_file_ids)

    # In some files (e.g. NMarioModel through NPikachuModel) every extern
    # entry is duplicated 2x in source for reasons that aren't visible from
    # the format alone — same (ptr_label, target_offset) appears twice in a
    # row. Torch's chain only encodes one slot per byte offset. Dedupe the
    # source-order list, pair with Torch, then propagate annotations to ALL
    # source occurrences (deduped or not).
    seen: dict[tuple[str, str], int] = {}
    src_to_uniq: list[int] = []
    uniq_indices: list[int] = []
    for src_i, (ptr, tgt, _) in enumerate(extern_meta):
        key = (ptr, tgt)
        if key in seen:
            src_to_uniq.append(seen[key])
        else:
            seen[key] = len(uniq_indices)
            uniq_indices.append(src_i)
            src_to_uniq.append(seen[key])
    n_uniq = len(uniq_indices)

    if n_src != n_torch and n_uniq != n_torch:
        out.skipped_reason = (
            f"length mismatch ({n_src} src / {n_uniq} uniq vs {n_torch} torch)"
        )
        return out

    # Pick which list to pair against Torch.
    use_dedup = (n_uniq == n_torch and n_src != n_torch)

    # Pair source[i] (or uniq[i]) with torch[i]. Decide what to write per line.
    new_lines = list(lines)
    for src_i, (idx, (ptr_label, target_str, existing)) in enumerate(zip(extern_indices, extern_meta)):
        torch_idx = src_to_uniq[src_i] if use_dedup else src_i
        torch_fid = torch.extern_file_ids[torch_idx]
        torch_name = names.get(torch_fid, "?")
        if existing is not None:
            if int(existing) == torch_fid:
                out.matched += 1
            else:
                out.conflict += 1
            continue  # don't rewrite — preserve existing annotations as-is

        new_annot = f"  # -> file {torch_fid} ({torch_name})"
        # Replace the trailing whitespace/comment with the new annotation.
        # The original line might have a stray comment we preserve nothing
        # of — extern lines don't carry side-info beyond the annotation.
        new_line = f"extern {ptr_label} {target_str}{new_annot}"
        new_lines[idx] = new_line
        out.written += 1

    if not check_only and out.written > 0:
        reloc_path.write_text("\n".join(new_lines) + "\n")

    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--battleship-o2r", type=Path, required=True)
    ap.add_argument("--reloc-table", type=Path, required=True)
    ap.add_argument("--reloc-dir", type=Path, required=True)
    ap.add_argument("--descriptions", type=Path, required=True,
                    help="decomp/tools/relocFileDescriptions.us.txt")
    ap.add_argument("--check", action="store_true",
                    help="verify existing annotations without writing")
    ap.add_argument("--verbose", action="store_true")
    args = ap.parse_args()

    table = parse_reloc_table(args.reloc_table)
    names = parse_file_names(args.descriptions)
    if not table:
        print(f"error: empty reloc table from {args.reloc_table}", file=sys.stderr)
        return 2

    zf = zipfile.ZipFile(args.battleship_o2r, "r")

    written_total = 0
    matched_total = 0
    conflict_total = 0
    skipped: list[tuple[int, str, str]] = []
    files_processed = 0

    for reloc_path in sorted(args.reloc_dir.glob("*.reloc")):
        m = RELOC_FILE_RE.match(reloc_path.name)
        if not m:
            continue
        fid = int(m.group(1))

        torch = torch_externs(zf, table, fid)
        if torch is None:
            continue
        if not torch.extern_file_ids:
            continue  # file has no externs

        outcome = annotate_one(reloc_path, torch, names, check_only=args.check)
        files_processed += 1
        written_total += outcome.written
        matched_total += outcome.matched
        conflict_total += outcome.conflict
        if outcome.skipped_reason:
            skipped.append((fid, reloc_path.name, outcome.skipped_reason))
        if args.verbose and (outcome.written or outcome.conflict):
            tag = "would write" if args.check else "wrote"
            print(f"  [{fid:4d}] {reloc_path.name}: "
                  f"{tag}={outcome.written} matched={outcome.matched} "
                  f"conflict={outcome.conflict}")

    print(f"=== Annotation summary{' (check only)' if args.check else ''} ===")
    print(f"  files processed (have externs)    : {files_processed}")
    print(f"  annotations {'would-be-' if args.check else ''}written : {written_total}")
    print(f"  existing annotations confirmed    : {matched_total}")
    print(f"  existing annotations conflicting  : {conflict_total}")
    print(f"  files skipped (length mismatch)   : {len(skipped)}")
    if skipped:
        print()
        for fid, name, reason in skipped[:10]:
            print(f"    {fid:4d}  {name}: {reason}")
        if len(skipped) > 10:
            print(f"    (+ {len(skipped) - 10} more)")

    if conflict_total > 0 and args.check:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
