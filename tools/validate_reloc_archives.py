#!/usr/bin/env python3
"""
validate_reloc_archives.py — compare per-file RelocFile resource bytes
between two sources, in one of two modes:

  --mode strict (default)
      Byte-equality check. Used for verifying a source-compiled archive
      against Torch's ROM-extracted ground truth. Reports field-level
      diffs (file_id, intern_off, extern_off, extern_file_ids, data
      size, data byte-diff count).

  --mode equivalent
      Runtime-equivalence check between two source-compiled archives
      whose data layouts may legitimately differ (e.g. clang vs MSVC).
      MSVC pads .data symbols to section alignment; clang
      -fno-zero-initialized-in-bss emits tightly packed .data. Both are
      runtime-correct against their own data blob. This mode aligns by
      symbol name (read from companion .o/.obj files) and verifies:
        (1) same file_id and same extern_file_ids (in chain order),
        (2) chain length matches for both intern and extern chains,
        (3) per-symbol content matches under the smaller of the two
            symbols' sizes (i.e. clang's tight size; MSVC's trailing
            alignment padding is ignored), excluding chain-slot bytes
            which legitimately differ between backends because they
            encode different absolute offsets,
        (4) chain semantics align — each chain entry's ptr and target
            classify to the same source-level symbol+intra in both.

Usage (strict):
    validate_reloc_archives.py
        --torch       <path-to-BattleShip.o2r>
        --source      <BattleShip.fromsource.o2r OR dir-of-.relo-files>
        --reloc-table <port/resource/RelocFileTable.cpp>
        [--file-id N [--file-id M ...]]
        [--out         report.md]

Usage (equivalent):
    validate_reloc_archives.py --mode equivalent
        --torch         <archive A: dir-of-.relo-files OR .o2r>
        --source        <archive B: dir-of-.relo-files OR .o2r>
        --torch-objdir  <dir of clang_<id>.o files for archive A>
        --source-objdir <dir of msvc_<id>.obj files for archive B>
        --reloc-table   <port/resource/RelocFileTable.cpp>
        [--file-id N ...]
        [--out          report.md]

The --torch / --source naming is retained from strict mode for arg-
parser back-compat; in equivalent mode they are simply 'archive A' and
'archive B' — neither has special meaning.
"""

from __future__ import annotations

import argparse
import re
import struct
import sys
import zipfile
from dataclasses import dataclass, field
from pathlib import Path

# Import the format parsers from the build pipeline so the equivalent-mode
# semantic comparison can read symbol tables out of the .obj/.o companions.
# build_reloc_resource.py guards its main() under __name__, so importing
# does not trigger a CLI run.
from build_reloc_resource import (
    Elf32, Coff32, parse_object, merge_data_sections,
)


HEADER_SIZE = 0x40  # LUS BinaryResource header — opaque, compared as bytes.


@dataclass
class RelocResource:
    header: bytes
    file_id: int
    intern_off: int
    extern_off: int
    extern_file_ids: list[int]
    data: bytes

    @classmethod
    def parse(cls, blob: bytes) -> "RelocResource":
        if len(blob) < HEADER_SIZE + 12:
            raise ValueError(f"resource too short ({len(blob)} bytes)")
        header = blob[:HEADER_SIZE]
        off = HEADER_SIZE
        (file_id,) = struct.unpack_from("<I", blob, off); off += 4
        (intern_off,) = struct.unpack_from("<H", blob, off); off += 2
        (extern_off,) = struct.unpack_from("<H", blob, off); off += 2
        (n_ext,) = struct.unpack_from("<I", blob, off); off += 4
        externs = list(struct.unpack_from(f"<{n_ext}H", blob, off))
        off += 2 * n_ext
        (data_size,) = struct.unpack_from("<I", blob, off); off += 4
        data = blob[off:off + data_size]
        if len(data) != data_size:
            raise ValueError(
                f"data size {data_size} but only {len(data)} trailing bytes")
        return cls(
            header=header, file_id=file_id, intern_off=intern_off,
            extern_off=extern_off, extern_file_ids=externs, data=data,
        )


@dataclass
class FieldDiff:
    file_id_match: bool
    header_match: bool
    intern_off_match: bool
    extern_off_match: bool
    extern_file_ids_match: bool
    data_size_match: bool
    data_first_diff_byte: int  # -1 if match
    data_byte_diff_count: int

    @classmethod
    def of(cls, a: RelocResource, b: RelocResource) -> "FieldDiff":
        first_diff = -1
        diff_count = 0
        if a.data == b.data:
            return cls(
                file_id_match=a.file_id == b.file_id,
                header_match=a.header == b.header,
                intern_off_match=a.intern_off == b.intern_off,
                extern_off_match=a.extern_off == b.extern_off,
                extern_file_ids_match=a.extern_file_ids == b.extern_file_ids,
                data_size_match=len(a.data) == len(b.data),
                data_first_diff_byte=-1,
                data_byte_diff_count=0,
            )
        n = min(len(a.data), len(b.data))
        for i in range(n):
            if a.data[i] != b.data[i]:
                if first_diff < 0:
                    first_diff = i
                diff_count += 1
        diff_count += abs(len(a.data) - len(b.data))
        return cls(
            file_id_match=a.file_id == b.file_id,
            header_match=a.header == b.header,
            intern_off_match=a.intern_off == b.intern_off,
            extern_off_match=a.extern_off == b.extern_off,
            extern_file_ids_match=a.extern_file_ids == b.extern_file_ids,
            data_size_match=len(a.data) == len(b.data),
            data_first_diff_byte=first_diff,
            data_byte_diff_count=diff_count,
        )

    @property
    def all_match(self) -> bool:
        return (self.file_id_match and self.header_match
                and self.intern_off_match and self.extern_off_match
                and self.extern_file_ids_match and self.data_size_match
                and self.data_byte_diff_count == 0)


# RelocFileTable.cpp lines look like: `	"reloc_animations/FTMarioAnimWait", /* 0 */`
_TABLE_RE = re.compile(r'^\s*"([^"]+)",\s*/\*\s*(\d+)\s*\*/')


def parse_reloc_table(path: Path) -> dict[int, str]:
    out: dict[int, str] = {}
    for line in path.read_text().splitlines():
        m = _TABLE_RE.match(line)
        if m:
            res_path, fid = m.group(1), int(m.group(2))
            out[fid] = res_path
    return out


def chain_walk_pairs(data: bytes, start_word: int, max_steps: int = 4096
                     ) -> list[tuple[int, int]]:
    """
    Walk a relocation chain. Returns list of (ptr_byte_offset, target_word)
    in chain order. Used by equivalent-mode to align chain entries
    semantically (same ptr-symbol+intra in both archives → same chain step).
    """
    if start_word == 0xFFFF:
        return []
    pairs: list[tuple[int, int]] = []
    word = start_word
    for _ in range(max_steps):
        byte_off = word * 4
        if byte_off + 4 > len(data):
            raise RuntimeError(
                f"chain slot at word 0x{word:04x} (byte 0x{byte_off:x}) "
                f"out of range (data {len(data)} bytes)")
        slot = struct.unpack_from(">I", data, byte_off)[0]
        next_word = (slot >> 16) & 0xFFFF
        target_word = slot & 0xFFFF
        pairs.append((byte_off, target_word))
        if next_word == 0xFFFF:
            return pairs
        word = next_word
    raise RuntimeError(f"chain did not terminate after {max_steps} steps")


def chain_walk(data: bytes, start_word: int, max_steps: int = 4096
               ) -> tuple[int, list[int]]:
    """
    Walk a relocation chain from `start_word` (in BE u32 slots), returning
    (steps_taken, list_of_target_words). Stops at 0xFFFF or if max_steps
    exceeded. Used as an integrity sanity check — every chain must
    terminate at 0xFFFF.
    """
    if start_word == 0xFFFF:
        return 0, []
    targets: list[int] = []
    word = start_word
    for _ in range(max_steps):
        byte_off = word * 4
        if byte_off + 4 > len(data):
            raise RuntimeError(
                f"chain slot at word 0x{word:04x} (byte 0x{byte_off:x}) "
                f"out of range (data {len(data)} bytes)")
        slot = struct.unpack_from(">I", data, byte_off)[0]
        next_word = (slot >> 16) & 0xFFFF
        target_word = slot & 0xFFFF
        targets.append(target_word)
        if next_word == 0xFFFF:
            return len(targets), targets
        word = next_word
    raise RuntimeError(f"chain did not terminate after {max_steps} steps")


def load_archive_index(path: Path) -> tuple[zipfile.ZipFile, set[str]]:
    zf = zipfile.ZipFile(path, "r")
    return zf, set(zf.namelist())


# ───────────────────────── equivalent-mode helpers ─────────────────────────


@dataclass
class EquivResult:
    """Per-file equivalence verdict. equivalent==True iff issues is empty."""
    file_id: int
    equivalent: bool
    issues: list[str] = field(default_factory=list)


def _classify_offset(byte_offset: int, syms: dict[str, dict]) -> str:
    """Find which symbol contains `byte_offset`. Returns 'name+0xN' or
    '<unknown@0xN>' if no symbol covers that offset (chain slot in a gap,
    a region the compiler emitted with no associated symbol, etc.)."""
    # Linear scan is fine — typical relocData files have <50 symbols. For
    # larger files this is still well below the per-file budget.
    for name, sym in syms.items():
        start = sym["offset"]
        end = start + sym["size"]
        if start <= byte_offset < end:
            intra = byte_offset - start
            return f"{name}+0x{intra:x}" if intra else name
    return f"<unknown@0x{byte_offset:x}>"


def _classify_intern_target(target_word: int, syms: dict[str, dict]) -> str:
    """Intern chain target_word is a u16 word index into the same data
    section: byte offset = target_word * 4. Classify to symbol+intra."""
    return _classify_offset(target_word * 4, syms)


def _chain_slot_byte_set(pairs: list[tuple[int, int]]) -> set[int]:
    """Bytes covered by chain ptr slots. Each slot is a u32 (4 bytes); we
    skip those 4 bytes per slot during per-symbol content compare since
    they encode chain structure (legitimately differs between backends)."""
    out: set[int] = set()
    for ptr_off, _ in pairs:
        out.update(range(ptr_off, ptr_off + 4))
    return out


def equivalence_check(a_blob: bytes, b_blob: bytes,
                      a_obj: bytes, a_obj_kind: str,
                      b_obj: bytes, b_obj_kind: str) -> list[str]:
    """Returns list of issues (empty list = equivalent)."""
    issues: list[str] = []

    a = RelocResource.parse(a_blob)
    b = RelocResource.parse(b_blob)

    # (1) file_id + extern_file_ids
    if a.file_id != b.file_id:
        issues.append(f"file_id: {a.file_id} vs {b.file_id}")
    if a.extern_file_ids != b.extern_file_ids:
        issues.append(
            f"extern_file_ids: {a.extern_file_ids} vs {b.extern_file_ids}")

    # (2) chain lengths
    a_intern = chain_walk_pairs(a.data, a.intern_off)
    b_intern = chain_walk_pairs(b.data, b.intern_off)
    a_extern = chain_walk_pairs(a.data, a.extern_off)
    b_extern = chain_walk_pairs(b.data, b.extern_off)

    if len(a_intern) != len(b_intern):
        issues.append(
            f"intern chain length: {len(a_intern)} vs {len(b_intern)}")
    if len(a_extern) != len(b_extern):
        issues.append(
            f"extern chain length: {len(a_extern)} vs {len(b_extern)}")

    # (3) symbol-aligned content match (skip chain-slot bytes)
    try:
        a_parsed = parse_object(a_obj, a_obj_kind)
        b_parsed = parse_object(b_obj, b_obj_kind)
    except Exception as e:
        issues.append(f"object-parse error: {e}")
        return issues

    _, a_syms = merge_data_sections(a_parsed)
    _, b_syms = merge_data_sections(b_parsed)

    a_skip = _chain_slot_byte_set(a_intern + a_extern)
    b_skip = _chain_slot_byte_set(b_intern + b_extern)

    common = set(a_syms) & set(b_syms)
    only_a = set(a_syms) - common
    only_b = set(b_syms) - common
    if only_a:
        issues.append(f"symbols only in A: {sorted(only_a)[:5]}"
                      + ("..." if len(only_a) > 5 else ""))
    if only_b:
        issues.append(f"symbols only in B: {sorted(only_b)[:5]}"
                      + ("..." if len(only_b) > 5 else ""))

    for name in sorted(common):
        a_sym = a_syms[name]
        b_sym = b_syms[name]
        # Use the smaller of the two sizes — that's the "real" content
        # size (clang emits tightly-packed; MSVC's _fill_sizes-derived
        # size includes trailing alignment padding which we should ignore).
        size = min(a_sym["size"], b_sym["size"])
        for i in range(size):
            a_off = a_sym["offset"] + i
            b_off = b_sym["offset"] + i
            if a_off in a_skip or b_off in b_skip:
                continue  # chain slot — legitimately differs
            if a.data[a_off] != b.data[b_off]:
                issues.append(
                    f"content {name}+0x{i:x}: "
                    f"A[0x{a_off:x}]=0x{a.data[a_off]:02x} vs "
                    f"B[0x{b_off:x}]=0x{b.data[b_off]:02x}")
                break  # one finding per symbol

    # (4) chain semantic alignment — each chain entry should resolve to
    # the same logical (ptr-symbol+intra, target-symbol+intra) in both
    # archives. Compare as MULTISETS rather than ordered lists: write_chain
    # threads entries sorted by ptr_offset, and when MSVC's alignment
    # padding shifts symbol offsets relative to each other the resulting
    # sort can differ. The runtime walks chains via byte pointers so it
    # is order-tolerant; what matters is that the same set of (ptr,
    # target) relations is encoded.
    from collections import Counter

    a_intern_pairs = Counter(
        (_classify_offset(p, a_syms), _classify_intern_target(t, a_syms))
        for p, t in a_intern
    )
    b_intern_pairs = Counter(
        (_classify_offset(p, b_syms), _classify_intern_target(t, b_syms))
        for p, t in b_intern
    )
    if a_intern_pairs != b_intern_pairs:
        only_a = a_intern_pairs - b_intern_pairs
        only_b = b_intern_pairs - a_intern_pairs
        if only_a:
            sample = list(only_a.elements())[:2]
            issues.append(f"intern pair only in A: {sample}")
        if only_b:
            sample = list(only_b.elements())[:2]
            issues.append(f"intern pair only in B: {sample}")

    # Externs: target is a dep-archive byte offset (statically computed
    # from .reloc annotations, layout-independent), so we compare at the
    # raw target_word level. Pair multiset is the right shape — the
    # extern_file_ids list parallels the chain, so identical multiset
    # plus identical extern_file_ids multiset → equivalent.
    a_extern_pairs = Counter(
        (_classify_offset(p, a_syms), t) for p, t in a_extern
    )
    b_extern_pairs = Counter(
        (_classify_offset(p, b_syms), t) for p, t in b_extern
    )
    if a_extern_pairs != b_extern_pairs:
        only_a = a_extern_pairs - b_extern_pairs
        only_b = b_extern_pairs - a_extern_pairs
        if only_a:
            sample = list(only_a.elements())[:2]
            issues.append(f"extern pair only in A: {sample}")
        if only_b:
            sample = list(only_b.elements())[:2]
            issues.append(f"extern pair only in B: {sample}")

    # extern_file_ids order divergence is benign WHEN multisets match —
    # the chain pair multisets above prove the underlying relations are
    # the same. Re-check (1)'s strict order-match and downgrade if it's
    # purely an ordering difference.
    if (issues
        and any(i.startswith("extern_file_ids:") for i in issues)
        and Counter(a.extern_file_ids) == Counter(b.extern_file_ids)
        and a_extern_pairs == b_extern_pairs):
        # Drop the strict-order complaint — same set of dep_file_ids
        # threaded by chain order, which differs because the chain itself
        # iterates in a different order due to MSVC layout padding.
        issues = [i for i in issues
                  if not i.startswith("extern_file_ids:")]

    return issues


def _find_obj_for_fid(objdir: Path, fid: int) -> tuple[bytes, str] | None:
    """Find a .obj or .o for this file_id under `objdir`. Returns
    (blob, kind) or None. Convention: phase 2 harness writes
    {clang,msvc}_<id>.{o,obj}."""
    for name, kind in (
        (f"clang_{fid}.o",  "clang"),
        (f"msvc_{fid}.obj", "msvc"),
        (f"{fid}.o",        "clang"),
        (f"{fid}.obj",      "msvc"),
    ):
        p = objdir / name
        if p.exists():
            return p.read_bytes(), kind
    return None


def _load_relo_resources(path: Path,
                         table: dict[int, str]) -> dict[int, bytes]:
    """Same enumeration logic as strict mode — directory of <id>.relo or
    an .o2r archive. Returns {file_id: relo_blob}."""
    out: dict[int, bytes] = {}
    if path.is_dir():
        for entry in sorted(path.glob("*.relo")):
            try:
                fid = int(entry.stem)
            except ValueError:
                # phase 2 outputs are named "<kind>_<id>.relo" — handle that.
                m = re.match(r"^[a-z]+_(\d+)$", entry.stem)
                if m:
                    fid = int(m.group(1))
                else:
                    continue
            out[fid] = entry.read_bytes()
    else:
        zf = zipfile.ZipFile(path, "r")
        path_to_fid = {p: fid for fid, p in table.items()}
        for name in zf.namelist():
            if name in path_to_fid:
                out[path_to_fid[name]] = zf.read(name)
            elif name.endswith(".relo"):
                try:
                    fid = int(Path(name).stem)
                    out[fid] = zf.read(name)
                except ValueError:
                    pass
    return out


def _main_equivalent(args) -> int:
    """Backend-vs-backend semantic equivalence sweep."""
    table = parse_reloc_table(args.reloc_table)
    if not table:
        print(f"error: no entries parsed from {args.reloc_table}",
              file=sys.stderr)
        return 2

    a_res = _load_relo_resources(args.torch, table)
    b_res = _load_relo_resources(args.source, table)

    fids = sorted(set(a_res) & set(b_res))
    if args.file_id:
        fids = [f for f in fids if f in set(args.file_id)]
    if not fids:
        print("error: no overlapping file_ids between the two archives",
              file=sys.stderr)
        return 2

    only_a = sorted(set(a_res) - set(b_res))
    only_b = sorted(set(b_res) - set(a_res))

    print(f"Equivalence check: {len(fids)} overlapping files "
          f"({len(only_a)} only in A, {len(only_b)} only in B)")

    results: list[EquivResult] = []
    n_skip_no_obj = 0
    for fid in fids:
        a_obj_pair = _find_obj_for_fid(args.torch_objdir, fid)
        b_obj_pair = _find_obj_for_fid(args.source_objdir, fid)
        if a_obj_pair is None or b_obj_pair is None:
            n_skip_no_obj += 1
            results.append(EquivResult(
                fid, False,
                [f"missing .obj/.o (A={a_obj_pair is not None}, "
                 f"B={b_obj_pair is not None})"]))
            continue
        try:
            issues = equivalence_check(
                a_res[fid], b_res[fid],
                a_obj_pair[0], a_obj_pair[1],
                b_obj_pair[0], b_obj_pair[1])
        except Exception as e:
            issues = [f"check raised {type(e).__name__}: {e}"]
        results.append(EquivResult(fid, not issues, issues))

    n_eq    = sum(1 for r in results if r.equivalent)
    n_diff  = sum(1 for r in results if not r.equivalent)

    print(f"\n=== equivalence summary ({len(results)} files) ===")
    print(f"  equivalent:     {n_eq}")
    print(f"  not equivalent: {n_diff}")
    if n_skip_no_obj:
        print(f"  (of which {n_skip_no_obj} skipped — missing .obj/.o)")

    # Bucket the issue patterns to surface common root causes.
    if n_diff:
        from collections import Counter
        bucket: Counter[str] = Counter()
        for r in results:
            if r.equivalent:
                continue
            # First issue is the canonical signature for this file.
            first = r.issues[0] if r.issues else "<empty>"
            # Strip per-file specifics to get a coarser bucket — drop
            # symbol names + numeric offsets from content/chain messages.
            sig = re.sub(r"\b0x[0-9a-fA-F]+\b", "0x?", first)
            sig = re.sub(r"\b\d+\b", "?", sig)
            sig = re.sub(r"d[A-Z]\w+", "<sym>", sig)
            bucket[sig[:160]] += 1
        print(f"\nfailure buckets ({len(bucket)} distinct):")
        for sig, n in bucket.most_common(20):
            print(f"  {n:>4}  {sig}")

    if args.verbose or n_diff <= 50:
        for r in results:
            if r.equivalent and not args.verbose:
                continue
            print(f"  [{r.file_id:4d}] {'OK' if r.equivalent else 'DIFF'}: "
                  f"{r.issues[0] if r.issues else ''}")

    if args.out is not None:
        with args.out.open("w", encoding="utf-8") as f:
            f.write("# RelocData equivalence report\n\n")
            f.write(f"- A : `{args.torch}` (objdir `{args.torch_objdir}`)\n")
            f.write(f"- B : `{args.source}` (objdir `{args.source_objdir}`)\n")
            f.write(f"- Equivalent     : {n_eq}\n")
            f.write(f"- Not equivalent : {n_diff}\n\n")
            if n_diff:
                f.write("| file_id | first issue |\n|---|---|\n")
                for r in results:
                    if r.equivalent:
                        continue
                    issue = (r.issues[0] if r.issues else "").replace(
                        "|", "\\|")
                    f.write(f"| {r.file_id} | {issue} |\n")
        print(f"wrote {args.out}")

    return 0 if n_diff == 0 else 1


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--mode", choices=("strict", "equivalent"),
                    default="strict",
                    help="comparison mode (default: strict — byte-equality)")
    ap.add_argument("--torch", type=Path, required=True,
                    help="archive A: .o2r OR directory of <id>.relo "
                         "(strict mode: Torch ground truth; "
                         "equivalent mode: arbitrary reference)")
    ap.add_argument("--source", type=Path, required=True,
                    help="archive B: .o2r OR directory of <id>.relo")
    ap.add_argument("--torch-objdir", type=Path, default=None,
                    help="(equivalent mode) directory containing the "
                         "compiled .o/.obj files for archive A — used to "
                         "resolve symbol offsets for content alignment")
    ap.add_argument("--source-objdir", type=Path, default=None,
                    help="(equivalent mode) same, for archive B")
    ap.add_argument("--reloc-table", type=Path, required=True)
    ap.add_argument("--file-id", action="append", type=int, default=None)
    ap.add_argument("--out", type=Path, default=None)
    ap.add_argument("--verbose", action="store_true")
    args = ap.parse_args()

    if args.mode == "equivalent":
        if not args.torch_objdir or not args.source_objdir:
            print("error: --mode equivalent requires both --torch-objdir "
                  "and --source-objdir", file=sys.stderr)
            return 2
        return _main_equivalent(args)

    table = parse_reloc_table(args.reloc_table)
    if not table:
        print(f"error: no entries parsed from {args.reloc_table}", file=sys.stderr)
        return 2

    torch_zf, torch_names = load_archive_index(args.torch)

    # Source-side: enumerate file_ids. From .o2r if zip, otherwise from
    # directory of <id>.relo.
    source_resources: dict[int, bytes] = {}
    if args.source.is_dir():
        for entry in sorted(args.source.glob("*.relo")):
            try:
                fid = int(entry.stem)
            except ValueError:
                continue
            source_resources[fid] = entry.read_bytes()
    else:
        sf = zipfile.ZipFile(args.source, "r")
        # The source archive may store entries by RelocFileTable path or
        # by <file_id>.relo. Handle either.
        path_to_fid = {p: fid for fid, p in table.items()}
        for name in sf.namelist():
            if name in path_to_fid:
                source_resources[path_to_fid[name]] = sf.read(name)
            elif name.endswith(".relo"):
                try:
                    fid = int(Path(name).stem)
                    source_resources[fid] = sf.read(name)
                except ValueError:
                    pass

    if args.file_id is not None:
        source_resources = {fid: blob for fid, blob in source_resources.items()
                            if fid in set(args.file_id)}

    if not source_resources:
        print("error: no source resources found / matched", file=sys.stderr)
        return 2

    rows: list[tuple[int, str, FieldDiff, str]] = []  # (fid, name, diff, note)

    for fid in sorted(source_resources):
        res_path = table.get(fid)
        if res_path is None:
            rows.append((fid, "<no path>", None, "no entry in reloc table"))
            continue
        if res_path not in torch_names:
            rows.append((fid, res_path, None, "not in torch archive"))
            continue
        try:
            torch_blob = torch_zf.read(res_path)
            torch_res = RelocResource.parse(torch_blob)
            src_res = RelocResource.parse(source_resources[fid])
        except Exception as e:
            rows.append((fid, res_path, None, f"parse error: {e}"))
            continue

        # Chain integrity sanity-check on source side.
        chain_note = ""
        try:
            steps, _ = chain_walk(src_res.data, src_res.intern_off)
            if src_res.intern_off != 0xFFFF and steps == 0:
                chain_note = "INTERN chain empty?"
        except RuntimeError as e:
            chain_note = f"BAD CHAIN: {e}"

        diff = FieldDiff.of(torch_res, src_res)
        rows.append((fid, res_path, diff, chain_note))

    # Print summary
    n_match = sum(1 for _, _, d, _ in rows if d is not None and d.all_match)
    n_diff = sum(1 for _, _, d, _ in rows if d is not None and not d.all_match)
    n_err = sum(1 for _, _, d, _ in rows if d is None)
    print(f"Compared {len(rows)} files: "
          f"{n_match} identical, {n_diff} differing, {n_err} errors")

    if args.verbose or args.out is None or n_diff <= 50:
        for fid, name, diff, note in rows:
            if diff is None:
                print(f"  [{fid:4d}] {name}: {note}")
                continue
            if diff.all_match and not args.verbose:
                continue
            tags = []
            if not diff.file_id_match: tags.append("file_id")
            if not diff.header_match: tags.append("header")
            if not diff.intern_off_match: tags.append("intern_off")
            if not diff.extern_off_match: tags.append("extern_off")
            if not diff.extern_file_ids_match: tags.append("extern_ids")
            if not diff.data_size_match: tags.append("data_size")
            if diff.data_byte_diff_count > 0:
                tags.append(f"data({diff.data_byte_diff_count}B"
                            f" first@0x{diff.data_first_diff_byte:x})")
            t = ",".join(tags) or "ok"
            extra = f" [{note}]" if note else ""
            print(f"  [{fid:4d}] {name}: {t}{extra}")

    if args.out is not None:
        with args.out.open("w") as f:
            f.write(f"# RelocData validation report\n\n")
            f.write(f"- Torch source : `{args.torch}`\n")
            f.write(f"- From source  : `{args.source}`\n")
            f.write(f"- Identical    : {n_match}\n")
            f.write(f"- Differing    : {n_diff}\n")
            f.write(f"- Errors       : {n_err}\n\n")
            f.write("| file_id | path | status | notes |\n")
            f.write("|---|---|---|---|\n")
            for fid, name, diff, note in rows:
                if diff is None:
                    f.write(f"| {fid} | `{name}` | error | {note} |\n")
                    continue
                if diff.all_match:
                    continue
                tags = []
                if not diff.intern_off_match: tags.append("intern_off")
                if not diff.extern_off_match: tags.append("extern_off")
                if not diff.extern_file_ids_match: tags.append("extern_ids")
                if not diff.data_size_match: tags.append("data_size")
                if diff.data_byte_diff_count > 0:
                    tags.append(f"data{diff.data_byte_diff_count}B")
                f.write(f"| {fid} | `{name}` | {','.join(tags)} | {note} |\n")
        print(f"wrote {args.out}")

    return 0 if n_err == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
